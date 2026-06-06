#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h" // Hereda de tu clase base Personaje
#include "jugador.h"
#include <QVector>
#include <QPixmap>
#include <QRect>

class Enemigo : public Personaje {
private:

    int daño;
    int tipoMovimiento;
    int estado;
    int memoriaJugador;
    bool escudoActivo;

    double distanciaJugador;
    bool jugadorVeloz;
    bool jugadorSobrecargado;
    bool jugadorArriba;
    bool jugadorAbajo;
    double ultimaPosYJugador;
    int contadorDisparosJugador;
    float tiempoEscudo;
    float cooldownEscudo;
    float tiempoOlvido;

    QPixmap sprites1;
    QPixmap sprites2;

    QVector<QPixmap> framesQuieto;
    QVector<QPixmap> framesCaminar;
    QVector<QPixmap> framesAtacar;
    QVector<QPixmap> framesGolpe;
    QVector<QPixmap> framesMuerte;
    QVector<QPixmap> framesEscudo;

    int frameActual;
    int contadorAnimacion;
    int velocidadAnimacion;

    void cortarHojasSprites();

    bool debeGenerarBalaEnemigo = false;
    float cooldownDisparoEnemigo = 0.0f;

public:
    Enemigo(double x, double y, int tipo);

    void percibir(Jugador *kael);
    void razonar();
    void actuar(Jugador *kael);
    void aprender();

    void mover() override;
    void atacar(Jugador *kael);

    int getDaño() const { return daño; }
    void modificarVida(int cantidad);
    int getVida() const { return vida; }
    bool estaMuerto() const { return (estado == 4); }
    void registrarImpacto(Jugador *kael);
    bool disparar();
    void contraataque();
    int getFrameActual() const { return frameActual; }

};

#endif // ENEMIGO_H
