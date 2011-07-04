#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dots2.xpm"
#include <QGraphicsScene>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    QGraphicsScene qgs = new QGraphicsScene();
    qgs.addPixmap(QPixmap(Dots2_xpm));


    ui->graphicsView->setScene(&qgs);
    ui->graphicsView->setShown(true);
    ui->graphicsView->show();
    delete ui;
}
