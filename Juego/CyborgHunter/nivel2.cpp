#include "Nivel2.h"
#include <QBrush>
#include <QPixmap>
#include <QDebug>

Nivel2::Nivel2(QObject *parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 1280, 720);

    dt = 0.02;
    tiempoRestante = 90.0;

    cargar();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);

    timer->start(20);
}

void Nivel2::cargar() {
    QPixmap fondo(":/Recursos/Fondos/Nivel2.png");
    setBackgroundBrush(QBrush(fondo.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
}

void Nivel2::actualizar() {

    tiempoRestante -= dt;
    if (tiempoRestante <= 0) {
        tiempoRestante = 0;
        timer->stop();
        qDebug() << "Se acabo el tiempo.";

    }

    // 2. MOVER JUGADOR (Usa tus ecuaciones con 'dt')
    // jugador->CalcularPosicion(dt); (Lo haremos cuando creemos a Kael)

    // 3. MOVER ANDROIDES (Usa tus ecuaciones con 'dt')
    // androide1->CalcularPosicion(dt);

    // Para probar que el tiempo corre finamente en tu consola de desarrollo:
    // qDebug() << "Tiempo restante del nivel:" << tiempoRestante;
}
