#ifndef FISICAS_H
#define FISICAS_H

class Fisicas
{
private:
    Fisicas();

public:
    static void tiroParabolico(float &x, float &y, float v0, float anguloGrados, float tiempo, float xInicial, float yInicial);

    static void retroceso(float &elongacion, float &velocidadY, float k, float b, float masa, float dt);

    static void resistencia(float &vx, float coeficienteFriccion, float masa, float dt);

    static void choqueElastico(float &v, float restitucion);

    static void caidaLibre(double &posY, float &velY, float gravedad, float dt);
};

#endif // FISICAS_H

