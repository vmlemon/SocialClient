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

QString iMultiPersonDemo = "<img src=\"C:\\Users\\Tyson Key\\Documents\\Pidgin Data\\icons\\2dbf45924bc85a5c16228ee3eec0eb4d7083ad61.jpg\" height=\"20\"\\> <span style=\"background-color:E9F09C; color:6E5E5E;\"><font face=\"S60 Sans\">Example User has gone offline</font></span> <img src=\"C:\\Users\\Tyson Key\\Documents\\Pidgin Data\\icons\\2dbf45924bc85a5c16228ee3eec0eb4d7083ad61.jpg\" height=\"20\"\\> <span style=\"background-color:EDCACA; color:6E5E5E;\"><font face=\"S60 Sans\">Another one bites the dust...</font></span>";

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
    ui->webView->setUrl(QUrl(iContentType + iStartRender + iMultiPersonDemo + iEndRender));
    ui->webView->show();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->webView->setUrl(QUrl(iContentType + iStartRender + arg1 + iEndRender));
    ui->webView->show();
}
