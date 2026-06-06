#include "CabezaRobot.h"
#include "fisicas.h"
#include <QDebug>

CabezaRobot::CabezaRobot(float x0, float y0, float v0, float anguloDeg, float escala, int canonOrigen)
    : QGraphicsPixmapItem(), x_inicial(x0), y_inicial(y0), origen(canonOrigen) {

    qDebug() << "Constructor cabeza";
    estaMuerto = false;
    tiempoTranscurrido = 0.0f;
    frameActual = 0;
    acumuladorAnimacion = 0.0f;
    velocidadInicial = v0;
    anguloG = anguloDeg;

    hojaSprites.load(":/Recursos/Sprites/Robots.png");
    anchoFrame = hojaSprites.width() / 4;
    altoFrame = hojaSprites.height() / 4;

    setScale(escala);
    setPos(x_inicial, y_inicial);

    this->setData(0, "cabeza");

    mapearFilaSprite();
    avanzarAnimacion(0.0f);
}

void CabezaRobot::mapearFilaSprite() {
    if (!estaMuerto) {
        filaActual = (origen == 0) ? 0 : 1;
    } else {
        filaActual = (origen == 0) ? 2 : 3;
    }
}

void CabezaRobot::actualizarPosicion(float dt) {
    tiempoTranscurrido += dt;

    float nuevoX = 0.0f;
    float nuevoY = 0.0f;

    Fisicas::tiroParabolico(nuevoX, nuevoY, velocidadInicial, anguloG, tiempoTranscurrido, x_inicial, y_inicial);

    setPos(nuevoX, nuevoY);
    avanzarAnimacion(dt);
}

void CabezaRobot::avanzarAnimacion(float dt) {
    acumuladorAnimacion += dt;

    if (acumuladorAnimacion >= 0.1f) {
        acumuladorAnimacion = 0.0f;
        if (estaMuerto) {
            if (frameActual < 3) frameActual++;
        } else {
            frameActual = (frameActual + 1) % 4;
        }
    }

    QPixmap framePix = hojaSprites.copy(frameActual * anchoFrame,
                                        filaActual * altoFrame,
                                        anchoFrame,
                                        altoFrame);
    setPixmap(framePix);
}

bool CabezaRobot::recibirDisparo() {
    if (!estaMuerto) {
        estaMuerto = true;
        frameActual = 0;
        mapearFilaSprite();
        return true;
    }
    return false;
}

CabezaRobot::~CabezaRobot() {}
