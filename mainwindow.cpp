#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebView>
#include <QWebFrame>
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QFile>

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

    QString icon = "<img src=\"" + iconUri + "\" height=\"24\" width=\"24\"/>";
    QString statusStart = QString().append("<span style=\"")
                                   .append("background-color:")
                                   .append(statusColour)
                                   .append("; color:6E5E5E;\">");
    QString statusFont("<font face=\"S60 Sans\">");
    QString statusEnd("</font></span>");

    QString result;

    result.append(icon).append(statusStart).append(statusFont).append(text).append(statusEnd).append("&nbsp;");
    //qDebug() << result;
    return result;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

    ui->webView->setUrl(QUrl(iContentType +
                             iStartRender +
                             BuildStatusItem(arg1, GetTwitterAvatarUri(QString(iTwitterFeed.readAll())), "E9F09C") +
                             BuildStatusItem(arg1, "C:\\Users\\Tyson Key\\Documents\\Pidgin Data\\icons\\2c884de1f6ad2379fc2b16eeab73a0b59623b6d5.png", "EDCACA") +
                             iEndRender));
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    ui->webView->show();
    qDebug() << ui->webView->url().toString();
}

QString MainWindow::GetTwitterAvatarUri(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    qDebug() << "Avatar URI: " << dataMap["profile_background_image_url"].toString();

    qDebug() << QString(iTwitterFeed.readAll());
    return dataMap["profile_background_image_url"].toString();
}

void MainWindow::resizeEvent(QResizeEvent *aEvent) {
    ui->webView->resize(aEvent->size().width(), ui->webView->height());
}

QString MainWindow::LoadDiskFeed(QString aFilePath) {

    QFile feedFile("./users.json");
    QTextStream feedStream(&feedFile);
}
