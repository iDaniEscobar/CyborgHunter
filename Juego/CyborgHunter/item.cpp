#include "Item.h"
#include <QDebug>

Item::Item(double x, double y, int tipoItem) {
    tipo = tipoItem;

    posx = x;
    posy = y;
    velocidadY = 0.0f;
    enSuelo = false;

    setPos(x, y);

    hojaCompleta.load(":/Recursos/Sprites/Items.png");

    cortarHojaSprites();
}

void Item::cortarHojaSprites() {

    int anchoSprite = 160;
    int altoSprite = 160;
    int columna = tipo - 1;

    QPixmap spriteItem = hojaCompleta.copy(QRect(columna * anchoSprite, 0, anchoSprite, altoSprite));

    setPixmap(spriteItem.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Item::aplicarEfecto(Jugador *kael) {
    if (!kael) return;

    switch (tipo) {
    case 1:
        kael->modificarVida(25);
        qDebug() << "Kael consumio Botiquin.";
        break;

    case 2:
        kael->activarModoVeloz();
        qDebug() << "Modo veloz activo.";
        break;

    case 3:
        kael->activarSobrecarga();
        qDebug() << "Sobrecarga activa.";
        break;

    default:
        break;
    }
}

void Item::caer(float dt) {
    if (enSuelo) return;

    float gravedad = 15.0f;
    Fisicas::caidaLibre(posy, velocidadY, gravedad, dt);

    if (posy >= 600) {
        posy = 600;
        enSuelo = true;
        velocidadY = 0;
    }
    setPos(posx, posy);
}
