#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsScene>
#include <QTimer>

class Nivel2 : public QGraphicsScene {
    Q_OBJECT
private:
    QTimer *timer;
    float tiempoRestante;
    float dt;

public:
    Nivel2(QObject *parent = nullptr);
    void cargar();

public slots:
    void actualizar();
};

#endif // NIVEL2_H
