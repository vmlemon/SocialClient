#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dots2.xpm"
#include <QGraphicsScene>
#include <QPixmap>
#include <QBitmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

QString iToRender = "<b>Bello!</b>";

MainWindow::~MainWindow()
{
    QGraphicsScene *qgs = new QGraphicsScene();
    QPixmap art = QBitmap::QPixmap(Dots2_xpm);
    qgs->addPixmap(art);

    ui->webView->setContent(iToRender.toAscii().data());
    ui->webView->show();
    ui->graphicsView->setScene(qgs);
    ui->graphicsView->setShown(true);
    ui->graphicsView->show();
    delete ui;
}
