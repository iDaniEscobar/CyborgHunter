#ifndef CABEZAROBOT_H
#define CABEZAROBOT_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class CabezaRobot : public QGraphicsPixmapItem {

public:
    CabezaRobot(float x0, float y0, float v0, float anguloDeg, float escala, int canonOrigen);
    ~CabezaRobot();

    void actualizarPosicion(float dt);
    bool recibirDisparo();
    bool verificarSiEstaMuerto() const { return estaMuerto; }

private:
    float x_inicial;
    float y_inicial;
    float velocidadInicial;
    float anguloG;
    float tiempoTranscurrido;

    QPixmap hojaSprites;
    int anchoFrame;
    int altoFrame;
    int frameActual;
    float acumuladorAnimacion;

    int origen;
    bool estaMuerto;
    int filaActual;

    void mapearFilaSprite();
    void avanzarAnimacion(float dt);
};

#endif // CABEZAROBOT_H
