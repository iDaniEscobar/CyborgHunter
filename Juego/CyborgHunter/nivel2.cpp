#include "Nivel2.h"
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <cstdlib>
#include <ctime>

Nivel2::Nivel2(QObject *parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 1280, 720);

    dt = 0.02;
    tiempoRestante = 90.0;

    teclaW_Presionada = false;
    teclaA_Presionada = false;
    teclaS_Presionada = false;
    teclaD_Presionada = false;

    balasDisponibles = 5;
    contadorCooldown = 0.0;
    contadorRecarga = 0.0;
    puedeDisparar = true;

    granadasDisponibles = 3;
    contadorRecargaGranada = 0.0f;

    sonidoDisparo = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);

    sonidoDisparo->setAudioOutput(salidaAudio);
    salidaAudio->setVolume(0.6);
    sonidoDisparo->setSource(QUrl("qrc:/Recursos/Sonidos/Laser.mp3"));

    vidaJugador = new QProgressBar();
    vidaJugador->setRange(0, 100);
    vidaJugador->setValue(100);
    vidaJugador->setStyleSheet("QProgressBar {border: 2px solid #aa00ff; text-align: center; background-color: #111; color: white; }"
                               "QProgressBar::chunk { background-color: #8800ff; }");

    proxyVidaJ = addWidget(vidaJugador);
    proxyVidaJ->setPos(20, 20);

    vidaJefe = new QProgressBar();
    vidaJefe->setRange(0, 200);
    vidaJefe->setValue(200);

    vidaJefe->setStyleSheet("QProgressBar {border: 2px solid #aa00ff; text-align: center; background-color: #111; color: white; }"
                            "QProgressBar::chunk { background-color: #8800ff; }");

    proxyVidaE = addWidget(vidaJefe);
    proxyVidaE->setPos(1050, 20);

    actualizarBalas();
    actualizarGranadas();

    cargar();

    kael = new Jugador(100, 500);
    addItem(kael);
    qDebug() << "Vida inicial del jugador:" << kael->getVida();

    jefe = new Enemigo(1000, 450, 2);
    addItem(jefe);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);

    timer->start(20);
}

void Nivel2::cargar() {
    QPixmap fondo(":/Recursos/Fondos/Nivel2.png");
    setBackgroundBrush(QBrush(fondo.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
}

void Nivel2::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        teclaW_Presionada = true;
    }
    else if (event->key() == Qt::Key_A) {
        teclaA_Presionada = true;
    }
    else if (event->key() == Qt::Key_S) {
        teclaS_Presionada = true;
    }
    else if (event->key() == Qt::Key_D) {
        teclaD_Presionada = true;
    }

    if (event->key() == Qt::Key_Space) {

        if (puedeDisparar && balasDisponibles > 0) {
            kael->setEstadoActual(2);
            balasDisponibles--;
            actualizarBalas();
            puedeDisparar = false;
            contadorCooldown = 2.0;
            qDebug() << "Balas restantes:" << balasDisponibles;
        } else if (balasDisponibles == 0) {
            qDebug() << "Sin balas";
        } else {
            qDebug() << "Sobrecalentamiento";
        }
    }

    if (event->key() == Qt::Key_E || event->key() == Qt::Key_R || event->key() == Qt::Key_F) {

        if (granadasDisponibles > 0) {
            granadasDisponibles--;
            actualizarGranadas();

            if(event->key() == Qt::Key_E){
                kael->lanzarGranada();

                Granada *granada = new Granada(kael->getPosx() + 60, kael->getPosy() + 20, 90.0f, 60.0f, 30);
                addItem(granada);
                listaProyectiles.append(granada);

                qDebug() << "¡Granada lanzada! Quedan:" << granadasDisponibles;
            }

            if(event->key() == Qt::Key_R){
                kael->lanzarGranada();

                Granada *granada = new Granada(kael->getPosx() + 60, kael->getPosy() + 20, 90.0f, 45.0f, 30);
                addItem(granada);
                listaProyectiles.append(granada);

                qDebug() << "¡Granada lanzada! Quedan:" << granadasDisponibles;
            }

            if(event->key() == Qt::Key_F){
                kael->lanzarGranada();

                Granada *granada = new Granada(kael->getPosx() + 60, kael->getPosy() + 20, 90.0f, 30.0f, 30);
                addItem(granada);
                listaProyectiles.append(granada);

                qDebug() << "¡Granada lanzada! Quedan:" << granadasDisponibles;
            }
        } else {
            qDebug() << "No quedan granadas.";
        }
    }


    QGraphicsScene::keyPressEvent(event);
}

void Nivel2::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        teclaW_Presionada = false;
    }
    else if (event->key() == Qt::Key_A) {
        teclaA_Presionada = false;
    }
    else if (event->key() == Qt::Key_S) {
        teclaS_Presionada = false;
    }
    else if (event->key() == Qt::Key_D) {
        teclaD_Presionada = false;
    }
    QGraphicsScene::keyReleaseEvent(event);
}
void Nivel2::actualizar() {

    cronometroSpawnItem += dt;
    if (cronometroSpawnItem >= 9.0f) {
        spawnearItemAleatorio();
        cronometroSpawnItem = 0.0f;
    }


    for (int i = 0; i < listaItems.size(); ++i) {
        listaItems[i]->caer(dt);

        if (listaItems[i]->collidesWithItem(kael)) {
            listaItems[i]->aplicarEfecto(kael);

            removeItem(listaItems[i]);
            delete listaItems[i];
            listaItems.removeAt(i);
            --i;
        }
    }

    tiempoRestante -= dt;

    if (tiempoRestante <= 0) {
        tiempoRestante = 0;
        timer->stop();
        qDebug() << "Se acabo el tiempo.";

    }

    kael->estMvmt(teclaW_Presionada, teclaA_Presionada, teclaS_Presionada, teclaD_Presionada);
    kael->mover();

    if (jefe) {
        jefe->percibir(kael);
        jefe->razonar();
        jefe->actuar(kael);
        vidaJefe->setValue(jefe->getVida());

        if (jefe->estaMuerto()) {
            qDebug() << "Jefe muerto...";
        }

        if (jefe->collidesWithItem(kael) && jefe->getVida() > 0) {

            if (kael->getEstadoActual() != 3) {

                kael->modificarVida(-jefe->getDaño());

                kael->golpeElastico(-8.0f, 0.0f);

                qDebug() << "¡Kael chocó con el cuerpo del Jefe! Choque elástico aplicado.";
            }
        }
    }

    for (int i = 0; i < listaProyectiles.size(); ++i) {
        Proyectil *p = listaProyectiles.at(i);
        p->mover();

        if (jefe && p->collidesWithItem(jefe) && jefe->getVida() > 0) {

            Granada *g = dynamic_cast<Granada*>(p);

            if (g) {

                g->explosion();
                if (g->pasoSuCiclo()) {
                    jefe->modificarVida(-g->getDaño());
                    jefe->registrarImpacto(kael);
                    removeItem(g);
                    listaProyectiles.removeAt(i);
                    delete g;
                    --i;
                    continue;
                }

            } else {

                jefe->modificarVida(-p->getDaño());
                jefe->registrarImpacto(kael);

                if (!jefe->estaMuerto()) {
                    jefe->contraataque();
                }

                removeItem(p);
                listaProyectiles.removeAt(i);
                delete p;
                --i;
            }
            qDebug() << "¡Impacto en el Jefe! Vida restante:" << jefe->getVida();
        }
    }

    int estadoDeMovimiento = kael->getEstadoActual();

    if (kael->disparar()) {

        Proyectil *bala = new Proyectil(kael->getPosx() + 80, kael->getPosy() + 45, 15.0, 1.0, 20);
        addItem(bala);
        listaProyectiles.append(bala);

        if (sonidoDisparo->playbackState() == QMediaPlayer::PlayingState) {
            sonidoDisparo->setPosition(0);
        }
        sonidoDisparo->play();




    }

    if (jefe && jefe->disparar()) {

        Proyectil *balaEnemiga = new Proyectil(jefe->getPosx() - 20, jefe->getPosy() + 100, 15.0, -1.0, 15);

        addItem(balaEnemiga);
        listaProyectiles.append(balaEnemiga);

        if (sonidoDisparo->playbackState() == QMediaPlayer::PlayingState) {
            sonidoDisparo->setPosition(0);
        }
        sonidoDisparo->play();
    }

    for (int i = 0; i < listaProyectiles.size(); ++i) {
        Proyectil *p = listaProyectiles.at(i);

        Granada *g = dynamic_cast<Granada*>(p);

        if (g && g->pasoSuCiclo()) {
            removeItem(g);
            listaProyectiles.removeAt(i);
            delete g;
            --i;
            continue;
        }

        p->mover();

        if (g && !g->explotando()) {


            if (g->getPosy() >= 620 || g->getPosx() > 1250 || g->getPosy() < 0) {
                g->explosion();
                continue;
            }

        }

        else if (!g) {

            if (p->collidesWithItem(kael)) {

                kael->modificarVida(-p->getDaño());
                kael->golpeElastico(-8.0f, 0.0f);
                qDebug() << "Kael fue impactado por una bala enemiga. Vida restante:" << kael->getVida();

                removeItem(p);
                listaProyectiles.removeAt(i);
                delete p;
                --i;
                continue;
            }
            if (p->getPosx() > 1280 || p->getPosx() < 0) {
                removeItem(p);
                listaProyectiles.removeAt(i);
                delete p;
                --i;
            }
        }



    }

    if (!puedeDisparar) {
        if (kael->isModoVelozActivo()) {
            contadorCooldown -= (dt * 2.5f);
        } else {
            contadorCooldown -= dt;
        }

        if (contadorCooldown <= 0) {
            puedeDisparar = true;
        }
    }


    if (balasDisponibles < 5) {
        contadorRecarga += dt;
        if (contadorRecarga >= 5.0) {
            balasDisponibles++;
            actualizarBalas();
            contadorRecarga = 0.0;
            qDebug() << "Munición recargada. Total:" << balasDisponibles;
        }
    } else {
        contadorRecarga = 0.0;
    }

    if (granadasDisponibles < 3) {
        contadorRecargaGranada += dt;

        if (contadorRecargaGranada >= 5.0f) {
            granadasDisponibles++;
            actualizarGranadas();
            contadorRecargaGranada = 0.0f;
            qDebug() << "Granada regenerada. Total:" << granadasDisponibles;
        }
    } else {

        contadorRecargaGranada = 0.0f;
    }

    kael->setEstadoActual(estadoDeMovimiento);
    vidaJugador->setValue(kael->getVida());
    kael->mover();
}

void Nivel2::spawnearItemAleatorio() {

    double xAleatoria = rand() % 576;
    double ySuperior = -40;
    int tipoAleatorio = (rand() % 3) + 1;

    Item* nuevoItem = new Item(xAleatoria, ySuperior, tipoAleatorio);
    addItem(nuevoItem);
    listaItems.append(nuevoItem);

    qDebug() << "Item generado en X:" << xAleatoria << " Tipo:" << tipoAleatorio;
}

void Nivel2::actualizarBalas() {
    for (auto icono : icnBalas) {
        removeItem(icono);
        delete icono;
    }
    icnBalas.clear();

    QPixmap spriteBala(":/Recursos/Sprites/Bala.png");
    QPixmap balaEscalada = spriteBala.scaled(30, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (int i = 0; i < balasDisponibles; ++i) {
        QGraphicsPixmapItem* nuevoIcono = new QGraphicsPixmapItem(balaEscalada);

        nuevoIcono->setPos(20 + (i * 30), 60);

        addItem(nuevoIcono);
        icnBalas.append(nuevoIcono);
    }
}

void Nivel2::actualizarGranadas() {
    for (auto icono : icnGranadas) {
        removeItem(icono);
        delete icono;
    }
    icnGranadas.clear();

    QPixmap spriteGranada(":/Recursos/Sprites/Granada.png");
    QPixmap granadaEscalada = spriteGranada.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (int i = 0; i < granadasDisponibles; ++i) {
        QGraphicsPixmapItem* nuevoIcono = new QGraphicsPixmapItem(granadaEscalada);

        nuevoIcono->setPos(20 + (i * 50), 100);

        addItem(nuevoIcono);
        icnGranadas.append(nuevoIcono);
    }
}
