#include "fisicas.h"
#include <cmath>

void Fisicas::tiroParabolico(float &x, float &y, float v0, float anguloGrados, float tiempo, float xInicial, float yInicial) {

    float anguloRadianes = anguloGrados * M_PI / 180.0;
    float g = 9.8;

    x = xInicial + (v0 * cos(anguloRadianes) * tiempo);
    y = yInicial - (v0 * sin(anguloRadianes) * tiempo) + (0.5 * g * tiempo * tiempo);
}

void Fisicas::resistencia(float &vx, float coeficienteFriccion, float masa, float dt) {
    float fuerzaResistencia = -coeficienteFriccion * vx;
    float aceleracion = fuerzaResistencia / masa;

    vx += aceleracion * dt;
}

void Fisicas::choqueElastico(float &v, float restitucion) {
    v = -v * restitucion;
}

void Fisicas::retroceso(float &elongacion, float &velocidadY, float k, float b, float masa, float dt) {

    float fuerzaResorte = -k * elongacion;
    float fuerzaAmortiguador = -b * velocidadY;
    float fuerzaTotal = fuerzaResorte + fuerzaAmortiguador;
    float aceleracionY = fuerzaTotal / masa;

    velocidadY += aceleracionY * dt;
    elongacion += velocidadY * dt;
}

void Fisicas::caidaLibre(double &posY, float &velY, float gravedad, float dt) {

    velY += gravedad * dt;
    posY += velY * dt;
}
