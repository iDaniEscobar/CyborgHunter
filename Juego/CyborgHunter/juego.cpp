#include "Juego.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>

#include "nivel1.h"
#include "nivel2.h"

Juego::Juego(QWidget *parent) : QGraphicsView(parent) {

    setFixedSize(1280, 720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    menuEscena = nullptr;
    nivelActual = nullptr;

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
    controlAudio->setVolume(0.2f);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();
}

void Juego::construirMenu() {

    QPushButton *btnCampoTiro = new QPushButton("CAMPO DE TIRO");
    QPushButton *btnEnfrentamiento = new QPushButton("ENFRENTAMIENTO");
    QPushButton *btnSalir = new QPushButton("SALIR");


    QString estiloBoton = "QPushButton { background-color: #00f0ff; color: black; font-weight: bold; border-radius: 5px; font-size: 18px; }"
                          "QPushButton:hover { background-color: #ff007f; color: white; }";

    btnCampoTiro->setStyleSheet(estiloBoton);
    btnEnfrentamiento->setStyleSheet(estiloBoton);
    btnSalir->setStyleSheet(estiloBoton);

    btnCampoTiro->resize(200, 50);
    btnEnfrentamiento->resize(200, 50);
    btnSalir->resize(200, 50);

    QGraphicsProxyWidget *proxyCampoTiro = menuEscena->addWidget(btnCampoTiro);
    proxyCampoTiro->setPos(540, 470);

    QGraphicsProxyWidget *proxyEnfrentamiento = menuEscena->addWidget(btnEnfrentamiento);
    proxyEnfrentamiento->setPos(540, 550);

    QGraphicsProxyWidget *proxySalir = menuEscena->addWidget(btnSalir);
    proxySalir->setPos(540, 630);


    connect(btnCampoTiro, &QPushButton::clicked, this, &Juego::mostrarMenuDificultad);
    connect(btnEnfrentamiento, &QPushButton::clicked, this, &Juego::clickEnfrentamiento);
    connect(btnSalir, &QPushButton::clicked, qApp, &QApplication::quit);
}

void Juego::clickEnfrentamiento() {
    if (musicaFondo) {
        musicaFondo->stop();
    }

    if (nivelActual) {
        nivelActual->deleteLater();
        nivelActual = nullptr;
    }


    nivelActual = new Nivel2(this);
    setScene(nivelActual);
    Nivel2 *nivel2Especifico = qobject_cast<Nivel2*>(nivelActual);
    connect(nivel2Especifico, &Nivel2::solicitarReiniciarNivel, this, &Juego::reiniciarNivelActual);
    connect(nivel2Especifico, &Nivel2::solicitarMenuPrincipal, this, &Juego::volverAlMenu);

    if (menuEscena) {
        menuEscena->deleteLater();
        menuEscena = nullptr;
    }

    musicaFondo->setSource(QUrl("qrc:/Recursos/Sonidos/Nivel2.mp3"));
    controlAudio->setVolume(0.2f);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();
}



void Juego::reiniciarNivelActual() {
    if (qobject_cast<Nivel1*>(nivelActual)) {
        mostrarMenuDificultad();
    }
    else if (qobject_cast<Nivel2*>(nivelActual)) {
        clickEnfrentamiento();
    }
}

void Juego::volverAlMenu() {
    qDebug() << "Controlador Juego: Volviendo al menú principal...";

    musicaFondo->stop();
    this->setMouseTracking(false);

    if (nivelActual) {
        nivelActual->deleteLater();
        nivelActual = nullptr;
    }

    mostrarPortada();
}

void Juego::mostrarMenuDificultad() {
    qDebug() << "Entró a mostrarMenuDificultad";
    qDebug() << "Entró a mostrarMenuDificultad";

    QGraphicsScene *nuevaEscena = new QGraphicsScene(this);
    nuevaEscena->setSceneRect(0,0,1280,720);

    QPixmap fondoN1(":/Recursos/Fondos/Nivel1.png");
    nuevaEscena->setBackgroundBrush(
        QBrush(fondoN1.scaled(1280,720))
        );

    setScene(nuevaEscena);

    if(menuEscena)
        menuEscena->deleteLater();

    menuEscena = nuevaEscena;
    qDebug() << "Construyendo menú dificultad";
    qDebug() << "Creando btnFacil";
    QPushButton *btnFacil = new QPushButton("MODO FÁCIL");
    qDebug() << "Creando btnDificil";
    QPushButton *btnDificil = new QPushButton("MODO DIFÍCIL");
    qDebug() << "Creando btnVolver";
    QPushButton *btnVolver = new QPushButton("VOLVER");

    QString estiloFacil = "QPushButton { background-color: #00ff66; color: black; font-weight: bold; border-radius: 5px; font-size: 16px; } QPushButton:hover { background-color: white; }";
    QString estiloDificil = "QPushButton { background-color: #ff0033; color: white; font-weight: bold; border-radius: 5px; font-size: 16px; } QPushButton:hover { background-color: white; color: black; }";
    QString estiloVolver = "QPushButton { background-color: #555; color: white; font-weight: bold; border-radius: 5px; font-size: 14px; }";

    btnFacil->setStyleSheet(estiloFacil);
    btnDificil->setStyleSheet(estiloDificil);
    btnVolver->setStyleSheet(estiloVolver);

    btnFacil->resize(220, 50);
    btnDificil->resize(220, 50);
    btnVolver->resize(100, 35);
    qDebug() << "Agregando btnFacil";

    menuEscena->addWidget(btnFacil)->setPos(530, 420);
    qDebug() << "Agregando btnDificil";
    menuEscena->addWidget(btnDificil)->setPos(530, 490);
    qDebug() << "Agregando btnVolver";
    menuEscena->addWidget(btnVolver)->setPos(590, 570);
    qDebug() << "Widgets agregados";

    connect(btnFacil, &QPushButton::clicked, this, &Juego::iniciarCampoTiroFacil);
    connect(btnDificil, &QPushButton::clicked, this, &Juego::iniciarCampoTiroDificil);
    connect(btnVolver, &QPushButton::clicked, this, &Juego::construirMenu);
    qDebug() << "Creo los connect";
}

void Juego::iniciarCampoTiroFacil() {
    if (nivelActual) { nivelActual->deleteLater(); nivelActual = nullptr; }

    nivelActual = new Nivel1(this, "facil");
    setScene(nivelActual);
    this->setMouseTracking(true);

    Nivel1 *n1 = qobject_cast<Nivel1*>(nivelActual);
    if (n1) {
        connect(n1, &Nivel1::solicitarReiniciarNivel, this, &Juego::reiniciarNivelActual);
        connect(n1, &Nivel1::solicitarMenuPrincipal, this, &Juego::volverAlMenu);
    }

    if (menuEscena) { menuEscena->deleteLater(); menuEscena = nullptr; }
    musicaN1();
}

void Juego::iniciarCampoTiroDificil() {
    if (nivelActual) { nivelActual->deleteLater(); nivelActual = nullptr; }

    nivelActual = new Nivel1(this, "dificil");
    setScene(nivelActual);
    this->setMouseTracking(true);

    Nivel1 *n1 = qobject_cast<Nivel1*>(nivelActual);
    if (n1) {
        connect(n1, &Nivel1::solicitarReiniciarNivel, this, &Juego::reiniciarNivelActual);
        connect(n1, &Nivel1::solicitarMenuPrincipal, this, &Juego::volverAlMenu);
    }

    if (menuEscena) { menuEscena->deleteLater(); menuEscena = nullptr; }
    musicaN1();
}

void Juego::musicaN1() {
    musicaFondo->stop();
    musicaFondo->setSource(QUrl("qrc:/Recursos/Sonidos/Nivel1.mp3"));
    controlAudio->setVolume(0.2f);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();
}
