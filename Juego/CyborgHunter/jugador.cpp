#include "Jugador.h"
#include <QDebug>
#include <QGraphicsScene>

class Proyectil;
class Nivel2;

Jugador::Jugador(double x, double y) : Personaje(x, y, 100, 5.0, ":/Recursos/Sprites/Kael_Quieto.png"){
    chipsRecolectados = 0;
    puntos = 0;
    modoVeloz = false;
    modoSobrecarga = false;

    frameActual = 0;
    estadoActual = 0;

    contadorAnimacion = 0;
    velocidadAnimacion = 5;

    hojaCompleta.load(":/Recursos/Sprites/Jugador.png");
    cortarHojaSprites();
    setPixmap(framesQuieto[0]);
}

void Jugador::cortarHojaSprites() {

    int altoSprite = 145;

    framesQuieto.append(hojaCompleta.copy(QRect(0, 0 * altoSprite, 90, altoSprite)));

    for(int i = 0; i < 4; ++i) {
        framesCaminar.append(hojaCompleta.copy(QRect(i * 105, 1 * altoSprite, 105, altoSprite)));
    }

    for(int i = 0; i < 4; ++i) {
        framesDisparar.append(hojaCompleta.copy(QRect(i * 123, 2 * altoSprite, 123, altoSprite)));
    }

    for(int i = 0; i < 4; ++i) {
        framesGolpe.append(hojaCompleta.copy(QRect(i * 110, 3 * altoSprite, 110, altoSprite)));
    }

    for(int i = 0; i < 4; ++i) {
        framesMuerte.append(hojaCompleta.copy(QRect(i * 160, 4 * altoSprite, 160, altoSprite)));
    }
}

void Jugador::mover() {

    if (estadoActual == 0) {
        frameActual = 0;
        contadorAnimacion = 0;
        setPixmap(framesQuieto[0]);
    }

    else if (estadoActual == 1) {
        contadorAnimacion++;

        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;

            frameActual++;
            if (frameActual >= framesCaminar.size()) {
                frameActual = 0;
            }

            setPixmap(framesCaminar[frameActual]);
        }
    }

    if (estadoActual == 2) {
        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;
            frameActual++;

            if (frameActual >= framesDisparar.size()) {
                frameActual = 0;
                estadoActual = 0;

                debeGenerarBala = true;
            } else {
                setPixmap(framesDisparar[frameActual]);
            }
        }
    }

    actualizar();
}

bool Jugador::disparar() {
    if (debeGenerarBala) {
        debeGenerarBala = false;
        qDebug() << "Jugador disparo";
        return true;
    }
    return false;


}

void Jugador::lanzarGranada() {
    qDebug() << "Granada lanzada";
}

void Jugador::activarPoder() {
    modoSobrecarga = true;
    qDebug() << "Modo SobreCarga activo";
}
