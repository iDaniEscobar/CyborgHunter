#include "Proyectil.h"
#include <cmath>

Proyectil::Proyectil(double x, double y, float vel, float dir, int dmg) {
    posx = x;
    posy = y;
    velocidad = vel;
    direccion = dir;
    daño = dmg;

    QPixmap pix(":/Recursos/Sprites/Disparo.png");
    setPixmap(pix.scaled(100, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    setPos(posx, posy);
}

void Proyectil::mover() {
    float dt = 0.02;

    double desplazamiento = velocidad * dt * 50 * direccion;
    posx += desplazamiento;
    setPos(posx, posy);
    update();
}

void Proyectil::verificarColision() {

}
