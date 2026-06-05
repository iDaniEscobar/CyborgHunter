#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"

class Jugador : public Personaje {
private:
    int chipsRecolectados;
    int puntos;
    bool modoVeloz;
    bool modoSobrecarga;

    QPixmap hojaCompleta;
    QVector<QPixmap> framesQuieto;
    QVector<QPixmap> framesCaminar;
    QVector<QPixmap> framesDisparar;
    QVector<QPixmap> framesGolpe;
    QVector<QPixmap> framesMuerte;

    int frameActual;
    int estadoActual;
    int contadorAnimacion;
    int velocidadAnimacion;

    void cortarHojaSprites();
    bool teclaW, teclaA, teclaS, teclaD;

    bool debeGenerarBala;
public:
    Jugador(double x, double y);

    void mover() override;
    bool disparar();
    void lanzarGranada();
    void activarPoder();
    void estMvmt(bool w, bool a, bool s, bool d);
    double getPosx() const { return posx; }
    void setPosx(double x) { posx = x; }
    double getPosy() const { return posy; }
    void setPosy(double y) { posy = y; }
    double getVelocidad() const { return velocidad; }
    void setEstadoActual(int estado) { estadoActual = estado; }
    int getEstadoActual() const { return estadoActual; }
};

#endif // JUGADOR_H
