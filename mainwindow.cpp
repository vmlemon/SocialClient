#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebView>
#include <QWebFrame>

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

    delete ui;
}

QString MainWindow::BuildStatusItem(QString aText, QString aIconUri, QString aStatusColour) {

    QString text = aText;
    QString iconUri = aIconUri;
    QString statusColour = aStatusColour;

    QString icon = "<img src=\"" + iconUri + "\" height=\"20\"/>";
    QString statusStart = QString().append("<span style=\"")
                                   .append("background-color:\"")
                                   .append(statusColour)
                                   .append("color:6E5E5E;\">");
    QString statusFont("<font face=\"S60 Sans\">");
    QString statusEnd("</font></span>");

    qDebug() << result;

    QString result;

    result.append(icon).append(statusStart).append(statusFont).append(text).append(statusEnd);

    return result;
}

void MainWindow::on_pushButton_clicked()
{
    //ui->webView->setContent(iToRender.toAscii().data());
    ui->webView->setUrl(QUrl(iContentType + iStartRender + "test" + iEndRender));
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    ui->webView->show();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->webView->setUrl(QUrl(iContentType + iStartRender + BuildStatusItem(arg1, "C:\\Users\\Tyson Key\\Documents\\Pidgin Data\\icons\\2dbf45924bc85a5c16228ee3eec0eb4d7083ad61.jpg", "E9F09C") + iEndRender));
    ui->webView->show();
}
