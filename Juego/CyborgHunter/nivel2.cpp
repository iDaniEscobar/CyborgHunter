#include "Nivel2.h"
#include <QBrush>
#include <QPixmap>
#include <QDebug>

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

    sonidoDisparo = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);

    sonidoDisparo->setAudioOutput(salidaAudio);
    salidaAudio->setVolume(0.6);
    sonidoDisparo->setSource(QUrl("qrc:/Recursos/Sonidos/Laser.mp3"));

    cargar();

    kael = new Jugador(100, 500);
    addItem(kael);

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
            puedeDisparar = false;
            contadorCooldown = 2.0;
            qDebug() << "Balas restantes:" << balasDisponibles;
        } else if (balasDisponibles == 0) {
            qDebug() << "Sin balas";
        } else {
            qDebug() << "Sobrecalentamiento";
        }
    }

    if (event->key() == Qt::Key_G) {
        kael->lanzarGranada();
        Granada *granada = new Granada(kael->getPosx() + 60, kael->getPosy() + 20, 90.0f, 45.0f, 60);
        addItem(granada);
        listaProyectiles.append(granada);
        qDebug() << "Lanzar granada.";
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

    tiempoRestante -= dt;
    if (tiempoRestante <= 0) {
        tiempoRestante = 0;
        timer->stop();
        qDebug() << "Se acabo el tiempo.";

    }

    kael->estMvmt(teclaW_Presionada, teclaA_Presionada, teclaS_Presionada, teclaD_Presionada);
    kael->mover();
    int estadoDeMovimiento = kael->getEstadoActual();

    if (kael->disparar()) {


        Proyectil *bala = new Proyectil(kael->getPosx() + 80, kael->getPosy() + 45, 15.0, 1.0, 25);
        addItem(bala);
        listaProyectiles.append(bala);

        if (sonidoDisparo->playbackState() == QMediaPlayer::PlayingState) {
            sonidoDisparo->setPosition(0);
        }
        sonidoDisparo->play();




    }

    for (int i = 0; i < listaProyectiles.size(); ++i) {
        Proyectil *p = listaProyectiles.at(i);

        p->mover();

        if (p->getPosx() > 1280 || p->getPosy() > 720) {
            removeItem(p);
            listaProyectiles.removeAt(i);
            delete p;
            --i;
        }
    }

    if (!puedeDisparar) {
        contadorCooldown -= dt;
        if (contadorCooldown <= 0) {
            puedeDisparar = true;
            qDebug() << "Listo para disparar";
        }
    }


    if (balasDisponibles < 5) {
        contadorRecarga += dt;
        if (contadorRecarga >= 5.0) {
            balasDisponibles++;
            contadorRecarga = 0.0;
            qDebug() << "Munición recargada. Total:" << balasDisponibles;
        }
    } else {
        contadorRecarga = 0.0;
    }
    kael->setEstadoActual(estadoDeMovimiento);
    kael->mover();
}
