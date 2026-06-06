#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int idFuente = QFontDatabase::addApplicationFont(":/Recursos/Fuente/CyberBlast.otf");

    if (idFuente == -1) {
        qDebug() << "No se pudo cargar la tipografía personalizada.";
    } else {
        QString nombreFamilia = QFontDatabase::applicationFontFamilies(idFuente).at(0);
        qDebug() << "Tipografía cargada con éxito" << nombreFamilia;

        QFont fuenteGlobal(nombreFamilia, 12);
        a.setFont(fuenteGlobal);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
