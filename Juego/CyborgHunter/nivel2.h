#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include "Jugador.h"

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
