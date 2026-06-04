#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "juego.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(1280, 720);

    Juego *juego = new Juego(this);

    setCentralWidget(juego);
}

MainWindow::~MainWindow()
{
    delete ui;
}
