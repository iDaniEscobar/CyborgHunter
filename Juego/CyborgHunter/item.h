#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRect>
#include "jugador.h"

class Item : public QGraphicsPixmapItem {
private:
    int tipo;
    double posy;
    double posx;
    float velocidadY;
    bool enSuelo;

    QPixmap hojaCompleta;
    void cortarHojaSprites();

public:

    Item(double x, double y, int tipoItem);
    void aplicarEfecto(Jugador *kael);
    void caer(float dt);
    int getTipo() const { return tipo; }
    bool estaEnSuelo() const { return enSuelo; }
};

#endif // ITEM_H
