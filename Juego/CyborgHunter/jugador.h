#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include "fisicas.h"

class Jugador : public Personaje {
private:
    int chipsRecolectados;
    int puntos;
    bool modoVeloz;
    float cronometroVeloz;
    bool modoSobrecarga;
    float cronometroSobrecarga;
    float velocidadX;

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
    void golpeElastico(float velImpulsoX, float velImpulsoY);
    void lanzarGranada();
    void activarSobrecarga();
    void activarModoVeloz();
    void modificarVida(int cantidad);
    void estMvmt(bool w, bool a, bool s, bool d);
    void setPosx(double x) { posx = x; }
    void setPosy(double y) { posy = y; }
    double getVelocidad() const { return velocidad; }
    void setEstadoActual(int estado) { estadoActual = estado; }
    int getEstadoActual() const { return estadoActual; }
    int getVida() const { return vida; }
    bool isModoVelozActivo() const { return modoVeloz; }
    bool isModoSobrecargaActivo() const { return modoSobrecarga; }
    int getFrameActual() const { return frameActual; }
};

#endif // JUGADOR_H
