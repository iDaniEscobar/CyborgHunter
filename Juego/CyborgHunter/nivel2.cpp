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
    int estadoDeMovimiento = 0;

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


    kael->setEstadoActual(estadoDeMovimiento);
    kael->mover();
}
