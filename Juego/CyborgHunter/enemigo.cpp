#include "Enemigo.h"
#include "fisicas.h"
#include <QDebug>
#include <cmath>

Enemigo::Enemigo(double x, double y, int tipo)
    : Personaje(x, y, (tipo == 2 ? 300 : 50), 2.5, "") {

    tipoMovimiento = tipo;
    daño = (tipoMovimiento == 2) ? 20 : 10;
    estado = 0;
    memoriaJugador = 0;
    escudoActivo = false;

    frameActual = 0;
    contadorAnimacion = 0;
    velocidadAnimacion = 6;

    sprites1.load(":/Recursos/Sprites/Enemigo1.png");
    sprites2.load(":/Recursos/Sprites/Enemigo2.png");

    cortarHojasSprites();
    setPixmap(framesQuieto[0]);
}

void Enemigo::cortarHojasSprites() {

    int altoSprite = 249;
    int anchoSprite = 332;

    framesQuieto.append(sprites1.copy(QRect(0, 0 * altoSprite, anchoSprite, altoSprite)));

    for(int i = 0; i < 4; ++i) {
        framesCaminar.append(sprites1.copy(QRect(i * anchoSprite, 1 * altoSprite, anchoSprite, altoSprite)));
    }
    for(int i = 0; i < 4; ++i) {
        framesAtacar.append(sprites1.copy(QRect(i * anchoSprite, 2 * altoSprite, anchoSprite, altoSprite)));
    }
    for(int i = 0; i < 4; ++i) {
        framesGolpe.append(sprites2.copy(QRect(i * anchoSprite, 0 * altoSprite, anchoSprite, altoSprite)));
    }
    for(int i = 0; i < 4; ++i) {
        framesMuerte.append(sprites2.copy(QRect(i * anchoSprite, 1 * altoSprite, anchoSprite, altoSprite)));
    }
    for(int i = 0; i < 4; ++i) {
        framesEscudo.append(sprites2.copy(QRect(i * anchoSprite, 2 * altoSprite, anchoSprite, altoSprite)));
    }
    framesAtacar[0].save("frame0.png");
    framesAtacar[1].save("frame1.png");
    framesAtacar[2].save("frame2.png");
    framesAtacar[3].save("frame3.png");
}

void Enemigo::percibir(Jugador *kael) {
    if (!kael) return;

    double distancia = sqrt(pow(kael->getPosx() - posx, 2) + pow(kael->getPosy() - posy, 2));


    if (tipoMovimiento == 1) {
        if (distancia < 200) estado = 1;
        else estado = 0;
        return;
    }

    if (distancia < 300 && kael->getVida() < 30) {
        estado = 1;
    }
    else if (distancia < 250 && estado == 1) {

        estado = 3;
    }
    else if (this->vida < 50) {
        estado = 2;
    }
    else {
        estado = 0;
    }
}

void Enemigo::razonar() {

    if (estado == 3) {
        escudoActivo = true;
    } else {
        escudoActivo = false;
    }

    aprender();
}

void Enemigo::actuar(Jugador *kael) {
    if (!kael) return;

    float dt = 0.02;
    float velocidadEnemigo = velocidad;

    switch (estado) {
    case 0:
        if (posx > 475) {
            if (kael->getPosx() > posx) posx += 0.7f;
            else posx -= 0.7f;

            contadorAnimacion++;
            if (contadorAnimacion >= velocidadAnimacion) {
                contadorAnimacion = 0;
                frameActual = (frameActual + 1) % framesCaminar.size();

                setPixmap(framesCaminar[frameActual]);
            }
        }
        else {

            if (frameActual != 0) {
                frameActual = 0;
                contadorAnimacion = 0;
                setPixmap(framesQuieto[0]);
            }
        }

        break;

    case 1:
        atacar(kael);

        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;
            frameActual = (frameActual + 1) % framesAtacar.size();
            setPixmap(framesAtacar[frameActual]);
        }
        break;

    case 2:
        if (kael->getPosx() > posx) posx -= (velocidadEnemigo * 1.5);
        else posx += (velocidadEnemigo * 1.5);

        setPixmap(framesCaminar[0]);
        break;

    case 3:
        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;
            frameActual = (frameActual + 1) % framesEscudo.size();
            setPixmap(framesEscudo[frameActual]);
        }
        break;

    case 4:
        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion + 2) {
            contadorAnimacion = 0;

            if (frameActual < framesMuerte.size() - 1) {
                frameActual++;
                setPixmap(framesMuerte[frameActual]);
            } else {

            }
        }
        return; //
    }

    if (posy < 380) posy = 380;
    if (posy > 570) posy = 570;
    if (posx > 1150) posx = 1150;
    if (posx < 370) posx = 370;

    setPos(posx, posy);
}

void Enemigo::aprender() {
    if (estado == 2) {
        memoriaJugador++;
    }
}

void Enemigo::mover() {
}

void Enemigo::atacar(Jugador *kael) {

    if (abs(kael->getPosx() - posx) < 50) {
        kael->modificarVida(-1);
    }
}

void Enemigo::modificarVida(int cantidad) {
    if (estado == 4) return;

    if (cantidad < 0) {
        if (escudoActivo) {
            qDebug() << "El Jefe bloqueo el ataque con su escudo";
            return;
        }
    }

    vida += cantidad;

    if (vida < 0) vida = 0;

    if (vida == 0) {
        estado = 4;
        frameActual = 0;
        contadorAnimacion = 0;
        escudoActivo = false;
        qDebug() << "El núcleo del Jefe Final fue liberado.";
    }

    int vidaMaxima = (tipoMovimiento == 2) ? 300 : 50;
    if (vida > vidaMaxima) vida = vidaMaxima;
}
