#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Personaje : public QGraphicsPixmapItem {
protected:
    double posx;
    double posy;
    int vida;
    double velocidad;

public:
    Personaje(double x, double y, int vidaInicial, double velInicial, const QString &rutaSprite);

    virtual void mover() = 0;
    void dañoRecibido(int cantidad);
    virtual void actualizar();

    int getVida() const { return vida; }
    double getPosx() const { return posx; }
    double getPosy() const { return posy; }
};

#endif // PERSONAJE_H
