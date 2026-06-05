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

    float dt = 0.02;

    if (modoSobrecarga) {
        cronometroSobrecarga -= dt;

        Fisicas::resistencia(velocidadX, 0.5f, 70.0f, dt);

        if (cronometroSobrecarga <= 0) {
            modoSobrecarga = false;
            velocidadX = velocidad;
            qDebug() << "Efecto sobrecarga terminado.";
        }
    }

    if (modoVeloz) {
        cronometroVeloz -= dt;
        if (cronometroVeloz <= 0) {
            modoVeloz = false;
            qDebug() << "Efecto modo veloz terminado.";
        }
    }

    bool seEstaMoviendo = teclaW || teclaA || teclaS || teclaD;
    float velFinal = modoSobrecarga ? velocidadX : velocidad;

    if (seEstaMoviendo && estadoActual != 2) {
        if (teclaD) posx += velFinal * dt * 50;
        else if (teclaA) posx -= velFinal * dt * 50;

        if (teclaS) posy += velFinal * dt * 50;
        else if (teclaW) posy -= velFinal * dt * 50;
    }

    if (estadoActual != 2 & estadoActual != 3) {
        if (seEstaMoviendo) {
            estadoActual = 1;
        } else {
            estadoActual = 0;
        }
    }

/*
    if (seEstaMoviendo && estadoActual != 2) {
        if (teclaD) posx += velocidad * dt * 50;
        else if (teclaA) posx -= velocidad * dt * 50;

        if (teclaS) posy += velocidad * dt * 50;
        else if (teclaW) posy -= velocidad * dt * 50;
    }*/

    if (posx < 0) posx = 0;
    if (posx > 1150) posx = 1150;
    if (posy < 380) posy = 380;
    if (posy > 570) posy = 570;

    if (estadoActual == 0) {
        frameActual = 0;
        contadorAnimacion = 0;
        setPixmap(framesQuieto[0]);
    }

    if (estadoActual == 1) {
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


    setPos(posx, posy);
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

void Jugador::estMvmt(bool w, bool a, bool s, bool d) {
    teclaW = w;
    teclaA = a;
    teclaS = s;
    teclaD = d;
}

void Jugador::lanzarGranada() {
    if (!framesGolpe.isEmpty()) {
        setPixmap(framesGolpe[0]);
    }
    qDebug() << "Granada lanzada";
}

void Jugador::modificarVida(int cantidad) {
    vida += cantidad;
    if (vida > 100) vida = 100;
    if (vida < 0) vida = 0;
}

void Jugador::activarSobrecarga() {
    modoSobrecarga = true;
    cronometroSobrecarga = 10.0f;
    velocidadX = velocidad-4;
}

void Jugador::activarModoVeloz() {
    modoVeloz = true;
    cronometroVeloz = 10.0f;
}
