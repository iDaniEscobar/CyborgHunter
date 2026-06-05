#ifndef GRANADA_H
#define GRANADA_H

#include "Proyectil.h"

class Granada : public Proyectil {
private:

    float angulo;
    float tiempoVuelo;

    float xInicial;
    float yInicial;

public:
    Granada(double x, double y, float vel, float ang, int dmg);

    void mover() override;
};

#endif // GRANADA_H
