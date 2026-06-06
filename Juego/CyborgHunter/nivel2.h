#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QProgressBar>
#include <QGraphicsProxyWidget>
#include <QList>
#include "Jugador.h"
#include "proyectil.h"
#include "granada.h"
#include "item.h"
#include "enemigo.h"

class Nivel2 : public QGraphicsScene {
    Q_OBJECT
private:
    QTimer *timer;
    float tiempoRestante;
    float dt;
    QGraphicsTextItem *textoTiempo;

    Jugador *kael;
    Enemigo *jefe;
    bool teclaW_Presionada;
    bool teclaA_Presionada;
    bool teclaS_Presionada;
    bool teclaD_Presionada;

    QList<Proyectil*> listaProyectiles;
    int balasDisponibles;
    float contadorCooldown;
    float contadorRecarga;
    bool puedeDisparar;
    QMediaPlayer *sonidoDisparo;
    QAudioOutput *salidaAudio;

    int granadasDisponibles;
    float contadorRecargaGranada;

    QProgressBar *vidaJugador;
    QGraphicsProxyWidget *proxyVidaJ;
    QProgressBar *vidaJefe;
    QGraphicsProxyWidget *proxyVidaE;

    QList<Item*> listaItems;
    float cronometroSpawnItem;
    void spawnearItemAleatorio();


    QList<QGraphicsPixmapItem*> icnBalas;
    QList<QGraphicsPixmapItem*> icnGranadas;

    void actualizarBalas();
    void actualizarGranadas();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public:
    Nivel2(QObject *parent = nullptr);
    void cargar();
    void mostrarPantallaFinJuego(bool victoria);

private slots:
    void clickReiniciar();
    void clickMenuPrincipal();
    void clickSalir();

public slots:
    void actualizar();

signals:
    void solicitarMenuPrincipal();
    void solicitarReiniciarNivel();
};

#endif // NIVEL2_H
