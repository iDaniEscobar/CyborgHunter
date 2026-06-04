#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QAudioOutput>

class Juego : public QGraphicsView {
    Q_OBJECT
private:
    QGraphicsScene *menuEscena;

    QMediaPlayer *musicaFondo;
    QAudioOutput *controlAudio;

    void construirMenu();

public:
    Juego(QWidget *parent = nullptr);

    void mostrarPortada();
    void iniciarNivel1();
    void iniciarNivel2();

public slots:
    void iniciarJuego();
};

#endif // JUEGO_H
