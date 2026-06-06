#include "nivel1.h"
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <cabezarobot.h>

Nivel1::Nivel1(QObject *parent, QString dificultad)
    : QGraphicsScene(parent), mododificultad(dificultad) {

    setSceneRect(0, 0, 1280, 720);

    QPixmap fondoImg(":/Recursos/Fondos/Nivel1.png");
    setBackgroundBrush(QBrush(fondoImg.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    configurarDificultad();
    cabezasDestruidas = 0;
    estaDisparando = false;

    tiempoRestante = 20.0f;
    nivelTerminado = false;

    sonidoDisparo = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);

    sonidoDisparo->setAudioOutput(salidaAudio);
    salidaAudio->setVolume(0.6);
    sonidoDisparo->setSource(QUrl("qrc:/Recursos/Sonidos/Laser.mp3"));

    textoContador = new QGraphicsTextItem();
    textoContador->setPlainText("KILLS: 0");
    textoContador->setDefaultTextColor(QColor("#00f0ff"));
    QFont fuenteHUD = textoContador->font();
    fuenteHUD.setPointSize(26);
    textoContador->setFont(fuenteHUD);
    textoContador->setPos(640 - textoContador->boundingRect().width()/2, 20);
    addItem(textoContador);

    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setPlainText("TIEMPO: 20");
    textoTiempo->setDefaultTextColor(Qt::yellow);
    QFont fuenteTiempo;
    fuenteTiempo.setPointSize(20);
    fuenteTiempo.setBold(true);
    textoTiempo->setFont(fuenteTiempo);
    textoTiempo->setPos(20,20);
    addItem(textoTiempo);

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
        velocidadBase = 200.0f;
        factorEscalaSprite = 0.6f;
        intervaloGeneracion = 1800;
    } else {
        velocidadBase = 300.0f;
        factorEscalaSprite = 0.4f;
        intervaloGeneracion = 1100;
    }
}

void Nivel1::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    //if (nivelTerminado) return;
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

    QPixmap frameActual = hojaSpritesPistola.copy(columna * anchoSpriteFrame, fila * altoSpriteFrame, anchoSpriteFrame, altoSpriteFrame);
    spritePistola->setPixmap(frameActual);

}

void Nivel1::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(nivelTerminado)
        return;

    if (sonidoDisparo->playbackState() == QMediaPlayer::PlayingState) {
        sonidoDisparo->setPosition(0);
    }
    sonidoDisparo->play();

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

                    if(cabezasDestruidas >= 10)
                    {
                        nivelTerminado = true;
                        mostrarPantallaFinJuego(true);
                        return;
                    }
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
    float yInicial = 600;
    int anguloAleatorio = 0;

    if (canonSeleccionado == 0) {
        xInicial = 100;
        anguloAleatorio = QRandomGenerator::global()->bounded(30, 75);
        qDebug() << "Generando cabezaI";
        qDebug() << "CañónI dispara cabeza con ángulo:" << anguloAleatorio;
    } else {
        xInicial = 1150;
        anguloAleatorio = QRandomGenerator::global()->bounded(105, 150);
        qDebug() << "Generando cabezaD";
        qDebug() << "CañónD dispara cabeza con ángulo:" << anguloAleatorio;
    }

    CabezaRobot *nuevaCabeza = new CabezaRobot(xInicial, yInicial, velocidadBase, anguloAleatorio, factorEscalaSprite, canonSeleccionado);
    addItem(nuevaCabeza);
}

void Nivel1::actualizarJuego() {
    if(nivelTerminado)
        return;

    tiempoRestante -= 0.02f;

    if(tiempoRestante <= 0)
    {
        tiempoRestante = 0;

        if(cabezasDestruidas < 10)
        {

            nivelTerminado = true;
            mostrarPantallaFinJuego(false);
            return;
        }

        return;
    }

    textoTiempo->setPlainText(
        QString("TIEMPO: %1")
            .arg((int)ceil(tiempoRestante))
        );

    QList<QGraphicsItem*> itemsEnEscena = items();
    for (QGraphicsItem *item : itemsEnEscena)
    {
        CabezaRobot *cabeza = dynamic_cast<CabezaRobot*>(item);

        if(cabeza)
        {
            cabeza->actualizarPosicion(0.05f);

            if(cabeza->listaParaEliminar())
            {
                removeItem(cabeza);
                delete cabeza;
                continue;
            }

            if(cabeza->y() > 750 || cabeza->x() < -100 || cabeza->x() > 1380)
            {
                removeItem(cabeza);
                delete cabeza;
            }
        }
    }
}


void Nivel1::mostrarPantallaFinJuego(bool victoria) {

    timerLoop->stop();
    timerSpawn->stop();


    QGraphicsRectItem *fondoOscuro = new QGraphicsRectItem(0, 0, 1280, 720);
    fondoOscuro->setBrush(QBrush(QColor(0, 0, 0, 180)));
    fondoOscuro->setPen(Qt::NoPen);
    fondoOscuro->setZValue(10);
    addItem(fondoOscuro);


    QGraphicsTextItem *textoTitulo = new QGraphicsTextItem();
    if (victoria) {
        textoTitulo->setPlainText("¡VICTORIA!");
        textoTitulo->setDefaultTextColor(QColor("#00ff66"));
    } else {
        textoTitulo->setPlainText("DERROTA");
        textoTitulo->setDefaultTextColor(QColor("#ff0033"));
    }

    QFont fuenteTitulo("Cyber Blast", 70, QFont::Bold);
    textoTitulo->setFont(fuenteTitulo);

    textoTitulo->setPos(640 - textoTitulo->boundingRect().width() / 2, 200);
    textoTitulo->setZValue(11);
    addItem(textoTitulo);

    QString estiloBotones = "QPushButton { background-color: #111; color: white; border: 2px solid #aa00ff; "
                            "border-radius: 5px; font-size: 18px; font-weight: bold; padding: 10px; }"
                            "QPushButton::hover { background-color: #8800ff; border-color: white; }";

    if (victoria) {

        QPushButton *btnMenu = new QPushButton("Menú Principal");
        btnMenu->setStyleSheet(estiloBotones);
        btnMenu->setGeometry(440, 350, 400, 50);
        QGraphicsProxyWidget *proxyMenu = addWidget(btnMenu);
        proxyMenu->setZValue(11);
        connect(btnMenu, &QPushButton::clicked, this, &Nivel1::clickMenuPrincipal);

        QPushButton *btnSalir = new QPushButton("Salir del Juego");
        btnSalir->setStyleSheet(estiloBotones);
        btnSalir->setGeometry(440, 430, 400, 50);
        QGraphicsProxyWidget *proxySalir = addWidget(btnSalir);
        proxySalir->setZValue(11);
        connect(btnSalir, &QPushButton::clicked, this, &Nivel1::clickSalir);
    }
    else {
        QPushButton *btnRepetir = new QPushButton("Reintentar Nivel");
        btnRepetir->setStyleSheet(estiloBotones);
        btnRepetir->setGeometry(440, 350, 400, 50);
        QGraphicsProxyWidget *proxyRepetir = addWidget(btnRepetir);
        proxyRepetir->setZValue(11);
        connect(btnRepetir, &QPushButton::clicked, this, &Nivel1::clickReiniciar);

        QPushButton *btnMenu = new QPushButton("Menú Principal");
        btnMenu->setStyleSheet(estiloBotones);
        btnMenu->setGeometry(440, 430, 400, 50);
        QGraphicsProxyWidget *proxyMenu = addWidget(btnMenu);
        proxyMenu->setZValue(11);
        connect(btnMenu, &QPushButton::clicked, this, &Nivel1::clickMenuPrincipal);
    }
}

void Nivel1::clickReiniciar()
{
    emit solicitarReiniciarNivel();
}

void Nivel1::clickMenuPrincipal()
{
    emit solicitarMenuPrincipal();
}

void Nivel1::clickSalir()
{
    qApp->quit();
}

Nivel1::~Nivel1()
{
    qDebug() << "Destruyendo Nivel1";

    if(timerLoop) timerLoop->stop();
    if(timerSpawn) timerSpawn->stop();
    if(timerDisparo) timerDisparo->stop();

    qDebug() << "Nivel1 destruido";
}
