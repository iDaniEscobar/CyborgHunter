#ifndef GRANADA_H
#define GRANADA_H

#include "Proyectil.h"
#include <QVector>
#include <QPixmap>
#include <QMediaPlayer>
#include <QAudioOutput>

class Granada : public Proyectil {
private:

    float angulo;
    float tiempoVuelo;
    float xInicial;
    float yInicial;

    bool enExplosion;
    bool cicloTerminado;

    QVector<QPixmap> framesExplosion;
    QPixmap hojaCompleta;
    void cortarHojaSprites();
    int frameActual;
    int contadorAnimacion;
    int velocidadAnimacion;

    QMediaPlayer *sonidoExplosion;
    QAudioOutput *salidaAudio;



public:
    Granada(double x, double y, float vel, float ang, int dmg);
    ~Granada();

    void mover() override;
    void explosion();
    bool pasoSuCiclo() const { return cicloTerminado; }
    bool explotando() const { return enExplosion; }
};

#endif // GRANADA_H
