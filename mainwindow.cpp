#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dots2.xbm"
#include <QGraphicsScene>
#include <QPixmap>
#include <QBitmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    QGraphicsScene *qgs = new QGraphicsScene();
    QPixmap art = QBitmap::fromData(QSize(Dots2_width, Dots2_height), Dots2_bits,QImage::Format_Mono);

    qgs->addPixmap(art);


    ui->graphicsView->setScene(qgs);
    ui->graphicsView->setShown(true);
    ui->graphicsView->show();
    delete ui;
}
