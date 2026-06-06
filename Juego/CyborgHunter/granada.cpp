#include "Granada.h"
#include "fisicas.h"
#include <QMediaPlayer>
#include <QAudioOutput>

Granada::Granada(double x, double y, float vel, float ang, int dmg)
    : Proyectil(x, y, vel, 1.0, dmg) {

    angulo = ang;
    tiempoVuelo = 0.0;

    xInicial = x;
    yInicial = y;

    enExplosion = false;
    cicloTerminado = false;
    frameActual = 0;
    contadorAnimacion = 0;
    velocidadAnimacion = 20;

    hojaCompleta.load(":/Recursos/Sprites/Explosion.png");
    cortarHojaSprites();

    QPixmap pix(":/Recursos/Sprites/Granada.png");
    setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    sonidoExplosion = new QMediaPlayer();
    salidaAudio = new QAudioOutput();
    sonidoExplosion->setAudioOutput(salidaAudio);
    salidaAudio->setVolume(0.8f);
    sonidoExplosion->setSource(QUrl("qrc:/Recursos/Sonidos/Explosion.mp3"));
}

Granada::~Granada() {
    delete sonidoExplosion;
    delete salidaAudio;
    qDebug() << "Memoria de granada liberada.";
}

void Granada::cortarHojaSprites() {

    int anchoSprite = 210;
    int altoSprite = 240;

    for(int i = 0; i < 4; ++i) {
        framesExplosion.append(hojaCompleta.copy(QRect(i * anchoSprite, 0, anchoSprite, altoSprite)));
    }

}

void Granada::mover() {

    if (enExplosion) {
        contadorAnimacion++;
        if (contadorAnimacion >= velocidadAnimacion) {
            contadorAnimacion = 0;
            frameActual++;

            if (frameActual >= framesExplosion.size()) {
                cicloTerminado = true;
            } else {

                setPixmap(framesExplosion[frameActual].scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                setPos(posx - 100, posy - 100);
            }
        }
        return;
    }

    float dt = 0.05;
    tiempoVuelo += dt;

    float tempX = 0.0f;
    float tempY = 0.0f;

    Fisicas::tiroParabolico(tempX, tempY, velocidad, angulo, tiempoVuelo, xInicial, yInicial);

    posx = tempX;
    posy = tempY;

    setPos(posx, posy);


}

void Granada::explosion() {
    if (!enExplosion) {
        enExplosion = true;
        sonidoExplosion->play();
    }
}
