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

class Nivel2 : public QGraphicsScene {
    Q_OBJECT
private:
    QTimer *timer;
    float tiempoRestante;
    float dt;

    Jugador *kael;
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

    QList<Item*> listaItems;
    float cronometroSpawnItem;
    void spawnearItemAleatorio();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public:
    Nivel2(QObject *parent = nullptr);
    void cargar();

public slots:
    void actualizar();
};

#endif // NIVEL2_H
