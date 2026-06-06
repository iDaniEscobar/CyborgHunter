// En Nivel1.h
#ifndef NIVEL1_H
#define NIVEL1_H

#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class Nivel1 : public QGraphicsScene {
    Q_OBJECT

public:
    Nivel1(QObject *parent = nullptr, QString dificultad = "facil");
    ~Nivel1();

    void mostrarPantallaFinJuego(bool victoria);

signals:
    void solicitarMenuPrincipal();
    void solicitarReiniciarNivel();

private slots:
    void actualizarJuego();
    void generarObjetivo();
    void apagarFogonazo();
    void clickReiniciar();
    void clickMenuPrincipal();
    void clickSalir();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:

    QString mododificultad;
    float velocidadBase;
    float factorEscalaSprite;
    int intervaloGeneracion;

    QTimer *timerLoop;
    QTimer *timerSpawn;
    QTimer *timerDisparo;
    float tiempoRestante;
    bool nivelTerminado;

    QGraphicsTextItem *textoContador;
    QGraphicsTextItem *textoTiempo;
    int cabezasDestruidas;

    QGraphicsPixmapItem *spritePistola;
    QPixmap hojaSpritesPistola;
    bool estaDisparando;
    int anchoSpriteFrame;
    int altoSpriteFrame;

    void configurarDificultad();
    void actualizarSpritePistola(qreal mouseX);
    QMediaPlayer *sonidoDisparo;
    QAudioOutput *salidaAudio;
};

#endif // NIVEL1_H
