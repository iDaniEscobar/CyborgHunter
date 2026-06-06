#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsPixmapItem>

class Proyectil : public QGraphicsPixmapItem {
protected:
    double posx;
    double posy;
    float velocidad;
    float direccion;
    int daño;

public:
    Proyectil(double x, double y, float vel, float dir, int dmg);

    virtual void mover();
    double getPosx() const { return posx; }
    double getPosy() const { return posy; }
    int getDaño() const { return daño; }
};

#endif // PROYECTIL_H
