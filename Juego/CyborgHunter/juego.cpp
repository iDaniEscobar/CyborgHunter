#include "Juego.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>

Juego::Juego(QWidget *parent) : QGraphicsView(parent) {

    setFixedSize(1280, 720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    menuEscena = nullptr;

    musicaFondo = new QMediaPlayer(this);
    controlAudio = new QAudioOutput(this);
    musicaFondo->setAudioOutput(controlAudio);

    mostrarPortada();
}

void Juego::mostrarPortada() {
    menuEscena = new QGraphicsScene(this);
    menuEscena->setSceneRect(0, 0, 1280, 720);

    QPixmap imgPortada(":/Recursos/Fondos/Portada.png");
    QPixmap fPortada = imgPortada.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    menuEscena->setBackgroundBrush(QBrush(fPortada));

    setScene(menuEscena);
    construirMenu();

    musicaFondo->setSource(QUrl("qrc:/Recursos/Sonidos/Portada.mp3"));
    controlAudio->setVolume(0.4);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();
}

void Juego::construirMenu() {

    QPushButton *btnIniciar = new QPushButton("INICIAR");
    QPushButton *btnSalir = new QPushButton("SALIR");


    QString estiloBoton = "QPushButton { background-color: #00f0ff; color: black; font-weight: bold; border-radius: 5px; font-size: 18px; }"
                          "QPushButton:hover { background-color: #ff007f; color: white; }";
    btnIniciar->setStyleSheet(estiloBoton);
    btnSalir->setStyleSheet(estiloBoton);

    btnIniciar->resize(200, 50);
    btnSalir->resize(200, 50);

    QGraphicsProxyWidget *proxyIniciar = menuEscena->addWidget(btnIniciar);
    proxyIniciar->setPos(540, 450);

    QGraphicsProxyWidget *proxySalir = menuEscena->addWidget(btnSalir);
    proxySalir->setPos(540, 530);


    connect(btnIniciar, &QPushButton::clicked, this, &Juego::iniciarJuego);
    connect(btnSalir, &QPushButton::clicked, qApp, &QApplication::quit);
}

void Juego::iniciarJuego() {
    if (musicaFondo) {
        musicaFondo->stop();
    }

    iniciarNivel2();
}

void Juego::iniciarNivel2() {

    if (menuEscena) {
        menuEscena->deleteLater();
        menuEscena = nullptr;
    }
}
