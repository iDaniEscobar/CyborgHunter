#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsPixmapItem>

class Proyectil : public QGraphicsPixmapItem {
private:
    double posx;
    double posy;
    float velocidad;
    float direccion;
    int daño;

public:
    Proyectil(double x, double y, float vel, float dir, int dmg);

    void mover();
    void verificarColision();
    double getPosx() const { return posx; }
};

#endif // PROYECTIL_H
