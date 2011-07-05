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

QString iContentType = "data:text/html,";
QString iStartRender = "<body bgcolor=\"black\" text=\"white\"><marquee>";
QString iEndRender = "</marquee> &nbsp; ";

MainWindow::~MainWindow()
{
    QGraphicsScene *qgs = new QGraphicsScene();
    QPixmap art = QBitmap::QPixmap(Dots2_xpm);
    qgs->addPixmap(art);


    ui->graphicsView->setScene(qgs);
    ui->graphicsView->setShown(true);
    ui->graphicsView->show();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //ui->webView->setContent(iToRender.toAscii().data());
    ui->webView->setUrl(QUrl(iContentType + iStartRender + "Enter content to begin..." + iEndRender));
    ui->webView->show();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->webView->setUrl(QUrl(iContentType + iStartRender + arg1 + iEndRender));
    ui->webView->show();
}
