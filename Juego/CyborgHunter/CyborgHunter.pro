QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enemigo.cpp \
    fisicas.cpp \
    granada.cpp \
    item.cpp \
    juego.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    nivel1.cpp \
    nivel2.cpp \
    personaje.cpp \
    proyectil.cpp

HEADERS += \
    enemigo.h \
    fisicas.h \
    granada.h \
    item.h \
    juego.h \
    jugador.h \
    mainwindow.h \
    nivel1.h \
    nivel2.h \
    personaje.h \
    proyectil.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc
