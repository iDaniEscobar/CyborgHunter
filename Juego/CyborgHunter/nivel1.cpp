#include "nivel1.h"
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <cabezarobot.h>

Nivel1::Nivel1(QObject *parent, QString dificultad)
    : QGraphicsScene(parent), mododificultad(dificultad) {

    setSceneRect(0, 0, 1280, 720);

    QPixmap fondoImg(":/Recursos/Fondos/Nivel1.png");
    setBackgroundBrush(QBrush(fondoImg.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    configurarDificultad();
    cabezasDestruidas = 0;
    estaDisparando = false;

    textoContador = new QGraphicsTextItem();
    textoContador->setPlainText("KILLS: 0");
    textoContador->setDefaultTextColor(QColor("#00f0ff"));
    QFont fuenteHUD = textoContador->font();
    fuenteHUD.setPointSize(26);
    textoContador->setFont(fuenteHUD);
    textoContador->setPos(640 - textoContador->boundingRect().width()/2, 20);
    addItem(textoContador);

    hojaSpritesPistola.load(":/Recursos/Sprites/Pistola.png");
    anchoSpriteFrame = hojaSpritesPistola.width() / 4;
    altoSpriteFrame = hojaSpritesPistola.height() / 2;

    spritePistola = new QGraphicsPixmapItem();

    spritePistola->setPos(610 - anchoSpriteFrame / 2, 780 - altoSpriteFrame);
    spritePistola->setZValue(10);
    addItem(spritePistola);

    actualizarSpritePistola(440);

    timerLoop = new QTimer(this);
    connect(timerLoop, &QTimer::timeout, this, &Nivel1::actualizarJuego);
    timerLoop->start(20);

    timerSpawn = new QTimer(this);
    connect(timerSpawn, &QTimer::timeout, this, &Nivel1::generarObjetivo);
    timerSpawn->start(intervaloGeneracion);

    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Nivel1::apagarFogonazo);

}

void Nivel1::configurarDificultad() {
    if (mododificultad == "facil") {
        velocidadBase = 90.0f;
        factorEscalaSprite = 1.0f;
        intervaloGeneracion = 1800;
    } else {
        velocidadBase = 26.0f;
        factorEscalaSprite = 0.6f;
        intervaloGeneracion = 1100;
    }
}

void Nivel1::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    actualizarSpritePistola(event->scenePos().x());
    QGraphicsScene::mouseMoveEvent(event);
}


void Nivel1::actualizarSpritePistola(qreal mouseX) {
    int fila = estaDisparando ? 1 : 0;
    int columna = 0;

    if (mouseX < 640) {

        columna = 0;
    } else {

        columna = 2;
    }

    QPixmap frameActual = hojaSpritesPistola.copy(columna * anchoSpriteFrame,
                                                  fila * altoSpriteFrame,
                                                  anchoSpriteFrame,
                                                  altoSpriteFrame);
    spritePistola->setPixmap(frameActual);

}

void Nivel1::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (!estaDisparando) {
        estaDisparando = true;
        actualizarSpritePistola(event->scenePos().x());

        timerDisparo->start(150);

        QPointF puntoClic = event->scenePos();
        QGraphicsItem *itemClickeado = itemAt(puntoClic, QTransform());

        if (itemClickeado && itemClickeado != spritePistola && itemClickeado->data(0).toString() == "cabeza") {
            CabezaRobot *robotAfectado = dynamic_cast<CabezaRobot*>(itemClickeado);
            if (robotAfectado) {
                if (robotAfectado->recibirDisparo()) {
                    cabezasDestruidas++;
                    textoContador->setPlainText(QString("KILLS: %1").arg(cabezasDestruidas));
                    textoContador->setPos(640 - textoContador->boundingRect().width()/2, 20);
                }
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Nivel1::apagarFogonazo() {
    timerDisparo->stop();
    estaDisparando = false;

    actualizarSpritePistola(640);
}

void Nivel1::generarObjetivo() {
    int canonSeleccionado = QRandomGenerator::global()->bounded(0, 2);

    float xInicial = 0;
    float yInicial = 650;
    int anguloAleatorio = 0;

    if (canonSeleccionado == 0) {
        xInicial = 50;
        anguloAleatorio = QRandomGenerator::global()->bounded(30, 75);
        qDebug() << "Generando cabezaI";
        qDebug() << "CañónI dispara cabeza con ángulo:" << anguloAleatorio;
    } else {
        xInicial = 1230;
        anguloAleatorio = QRandomGenerator::global()->bounded(105, 150);
        qDebug() << "Generando cabezaD";
        qDebug() << "CañónD dispara cabeza con ángulo:" << anguloAleatorio;
    }

    CabezaRobot *nuevaCabeza = new CabezaRobot(xInicial, yInicial, velocidadBase, anguloAleatorio, factorEscalaSprite, canonSeleccionado);
    addItem(nuevaCabeza);
}

void Nivel1::actualizarJuego() {
    QList<QGraphicsItem*> itemsEnEscena = items();
    for (QGraphicsItem *item : itemsEnEscena) {
        CabezaRobot *cabeza = dynamic_cast<CabezaRobot*>(item);
        if (cabeza) {
            cabeza->actualizarPosicion(0.02f);

            if (cabeza->y() > 750) {
                removeItem(cabeza);
                delete cabeza;
            }
        }
    }
}


Nivel1::~Nivel1() {}
