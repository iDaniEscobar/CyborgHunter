#include "Enemigo.h"
#include "fisicas.h"
#include <QDebug>
#include <cmath>

Enemigo::Enemigo(double x, double y, int tipo)
    : Personaje(x, y, (tipo == 2 ? 200 : 50), 2.5, "") {

    tipoMovimiento = tipo;
    daño = (tipoMovimiento == 2) ? 20 : 10;
    estado = 0;
    memoriaJugador = 0;
    escudoActivo = false;

    frameActual = 0;
    contadorAnimacion = 0;
    velocidadAnimacion = 6;

    jugadorArriba = false;
    jugadorAbajo = false;
    tiempoOlvido = 0.0f;

    ultimaPosYJugador = 0;
    contadorDisparosJugador = 0;
    tiempoEscudo = 0.0f;
    cooldownEscudo = 0.0f;

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
}

void Enemigo::percibir(Jugador *kael) {
    if (!kael) return;

    distanciaJugador = sqrt(pow(kael->getPosx() - posx, 2) + pow(kael->getPosy() - posy, 2));

    jugadorVeloz = kael->isModoVelozActivo();
    jugadorSobrecargado = kael->isModoSobrecargaActivo();

    jugadorArriba = kael->getPosy() < posy;
    jugadorAbajo = kael->getPosy() > posy;

}

void Enemigo::razonar()
{   if (vida <= 0 || estado == 4) {
        estado = 4;
        return;
    }

    if (estado == 5) {
        return;
    }

    if(escudoActivo)
    {
        tiempoEscudo -= 0.02f;

        if(tiempoEscudo <= 0)
        {
            escudoActivo = false;
            contadorDisparosJugador = 0;

            qDebug() << "Escudo desactivado";
        }
    }

    if(cooldownEscudo > 0)
    {
        cooldownEscudo -= 0.02f;
    }


    if(contadorDisparosJugador >= 2 &&
        !escudoActivo &&
        cooldownEscudo <= 0)
    {
        escudoActivo = true;
        tiempoEscudo = 1.0f;
        cooldownEscudo = 4.0f;

        qDebug() << "Escudo activado";
    }


    if (escudoActivo) {
        estado = 3;
    }

    else if (distanciaJugador < 50) {
        estado = 1;
    }

    else if (jugadorSobrecargado || (abs(posy - ultimaPosYJugador) < 80)) {
        contraataque();
        estado = 5;
    }


    else if (vida <= 50) {
        if (posx < 900) {
            estado = 2;
        } else {
            estado = 5;
        }
    }

    else if (posx <= 585) {
        estado = 0;
    }
    else {
        estado = 0;
    }

    aprender();
}

void Enemigo::actuar(Jugador *kael) {
    if (!kael) return;

    //float dt = 0.02;
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

        if(kael->getPosx() > posx)
            posx -= velocidadEnemigo;
        else
            posx += velocidadEnemigo;

        if(jugadorArriba)
            posy += 1.0;
        else
            posy -= 1.0;

        contadorAnimacion++;

        if(contadorAnimacion >= velocidadAnimacion)
        {
            contadorAnimacion = 0;
            frameActual = (frameActual + 1) % framesCaminar.size();

            setPixmap(framesCaminar[frameActual]);
        }

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
        break;

    case 5:
        if (cooldownDisparoEnemigo > 0) {
            cooldownDisparoEnemigo -= 0.02f;
        }

        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;
            frameActual++;

            if (frameActual >= framesAtacar.size()) {
                frameActual = 0;
                estado = 0;

                if (cooldownDisparoEnemigo <= 0) {
                    debeGenerarBalaEnemigo = true;
                    cooldownDisparoEnemigo = 2.0f;
                }
            } else {
                setPixmap(framesAtacar[frameActual]);
            }
        }
        break;
    }




    if (posy < 380) posy = 380;
    if (posy > 490) posy = 490;
    if (posx > 950) posx = 950;
    if (posx < 370) posx = 370;

    setPos(posx, posy);
}

void Enemigo::aprender()
{
    tiempoOlvido += 0.02f;

    if(tiempoOlvido >= 10.0f)
    {
        if(memoriaJugador > 0)
            memoriaJugador--;

        else if(memoriaJugador < 0)
            memoriaJugador++;

        tiempoOlvido = 0.0f;
    }

    if(memoriaJugador > 1)
    {
        posy += 1.5;
    }

    else if(memoriaJugador < -1)
    {
        posy -= 1;
    }
}

void Enemigo::mover() {
}

void Enemigo::atacar(Jugador *kael) {

    if (abs(kael->getPosx() - posx) < 150) {
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

    int vidaMaxima = (tipoMovimiento == 2) ? 200 : 50;
    if (vida > vidaMaxima) vida = vidaMaxima;
}

void Enemigo::registrarImpacto(Jugador *kael)
{
    if(!kael) return;

    if(kael->getPosy() - 45 < 475)
    {
        memoriaJugador++;
    }
    else
    {
        memoriaJugador--;
    }

    contadorDisparosJugador++;
}

bool Enemigo::disparar() {
    if (debeGenerarBalaEnemigo) {
        debeGenerarBalaEnemigo = false;
        qDebug() << "El Jefe Final generó un proyectil.";
        return true;
    }
    return false;
}

void Enemigo::contraataque() {
    if (escudoActivo || estado == 4) return;

    estado = 5;
    frameActual = 0;
    contadorAnimacion = 0;
    cooldownDisparoEnemigo = 0;
    qDebug() << "¡Jefe enfurecido! Contraataque activado por impacto.";
}
