#include "Granada.h"
#include "fisicas.h"

Granada::Granada(double x, double y, float vel, float ang, int dmg)
    : Proyectil(x, y, vel, 1.0, dmg) {

    angulo = ang;
    tiempoVuelo = 0.0;

    xInicial = x;
    yInicial = y;

    QPixmap pix(":/Recursos/Sprites/Granada.png");
    setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Granada::mover() {
    float dt = 0.05;
    tiempoVuelo += dt;

    float tempX = 0.0f;
    float tempY = 0.0f;

    Fisicas::tiroParabolico(tempX, tempY, velocidad, angulo, tiempoVuelo, xInicial, yInicial);

    posx = tempX;
    posy = tempY;

    setPos(posx, posy);
}
