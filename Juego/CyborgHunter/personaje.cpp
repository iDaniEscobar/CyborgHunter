#include "Personaje.h"

Personaje::Personaje(double x, double y, int vidaInicial, double velInicial, const QString &rutaSprite) {
    posx = x;
    posy = y;
    vida = vidaInicial;
    velocidad = velInicial;

    QPixmap pixmap(rutaSprite);
    QPixmap spriteEscalado = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(spriteEscalado);

    setPos(posx, posy);
}

void Personaje::dañoRecibido(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;
}

void Personaje::actualizar() {

    setPos(posx, posy);
}
