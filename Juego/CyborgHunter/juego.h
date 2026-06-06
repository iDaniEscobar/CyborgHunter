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
    QGraphicsScene *nivelActual;

    QMediaPlayer *musicaFondo;
    QAudioOutput *controlAudio;

    void musicaN1();

public:
    Juego(QWidget *parent = nullptr);

    void mostrarPortada();

private slots:

    void clickEnfrentamiento();
    void volverAlMenu();
    void reiniciarNivelActual();

    void mostrarMenuDificultad();
    void iniciarCampoTiroFacil();
    void iniciarCampoTiroDificil();
    void construirMenu();
};

#endif // JUEGO_H
