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
    bool seEstaMoviendo = teclaD_Presionada || teclaA_Presionada || teclaS_Presionada || teclaW_Presionada;

    int estadoDeMovimiento = kael->getEstadoActual();

    if (seEstaMoviendo) {
        estadoDeMovimiento = 1;

        if (teclaD_Presionada) kael->setPosx(kael->getPosx() + kael->getVelocidad() * dt * 50);
        else if (teclaA_Presionada) kael->setPosx(kael->getPosx() - kael->getVelocidad() * dt * 50);

        if (teclaS_Presionada) kael->setPosy(kael->getPosy() + kael->getVelocidad() * dt * 50);
        else if (teclaW_Presionada) kael->setPosy(kael->getPosy() - kael->getVelocidad() * dt * 50);
    }

    else if (estadoDeMovimiento != 2) {
        estadoDeMovimiento = 0;
    }

    if (teclaD_Presionada) {
        kael->setPosx(kael->getPosx() + kael->getVelocidad() * dt * 50);
        estadoDeMovimiento = 1;
    }
    else if (teclaA_Presionada) {
        kael->setPosx(kael->getPosx() - kael->getVelocidad() * dt * 50);
        estadoDeMovimiento = 1;
    }

    if (teclaS_Presionada) {

        kael->setPosy(kael->getPosy() + kael->getVelocidad() * dt * 50);
        estadoDeMovimiento = 1;
    }
    else if (teclaW_Presionada) {

        kael->setPosy(kael->getPosy() - kael->getVelocidad() * dt * 50);
        estadoDeMovimiento = 1;
    }

    if (kael->getPosx() < 0) {
        kael->setPosx(0);
    }

    if (kael->getPosx() > 1150) {
        kael->setPosx(1150);
    }


    if (kael->getPosy() < 380) {
        kael->setPosy(380);
    }

    if (kael->getPosy() > 570) {
        kael->setPosy(570);
    }

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
        Proyectil *bala = listaProyectiles.at(i);

        bala->mover();

        if (bala->getPosx() > 1280) {
            removeItem(bala);
            listaProyectiles.removeAt(i);
            delete bala;
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
