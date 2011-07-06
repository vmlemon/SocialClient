#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebView>
#include <QWebFrame>
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QBuffer>
#include <QFile>
#include <QMap>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

QString iContentType = "data:text/html,";
QString iStartRender = "<body bgcolor=\"black\" text=\"white\"><marquee>";
QString iEndRender = "</marquee> &nbsp; ";

/* Username, Data */
QMap<QString, QString> iSkypeCache;
QMap<QString, QString> iTwitterCache;

QString iNetworkData;

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
    qDebug() << arg1;
}

QString MainWindow::GetTwitterAvatarUri(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    qDebug() << "Avatar URI: " << dataMap["profile_image_url"].toString();
    return dataMap["profile_image_url"].toString();
}

QString MainWindow::GetTwitterLatestTweet(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();

    QVariantMap statusMap = dataMap["status"].toMap();

    qDebug() << "Latest Tweet: " << statusMap["text"].toString();
    return statusMap["text"].toString();
}

void MainWindow::resizeEvent(QResizeEvent *aEvent) {
    ui->webView->resize(aEvent->size().width(), ui->webView->height());
}

QString MainWindow::LoadDiskFeed(QString aFilePath) {

    QFile feedFile(aFilePath);
    QString feedLine;

    if (!feedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not read from " << aFilePath;
    }

    QTextStream feedStream(&feedFile);

    while (!feedStream.atEnd()) {
             feedLine = feedStream.readLine();
             //qDebug() << QString(feedLine);
    }
    return feedLine;
}

QString MainWindow::ParseSkypeStatus(QString aStatusData) {
    qDebug() << "aStatusData contains: " << aStatusData;
    int state = aStatusData.toInt();

    switch (state) {
        case EOffline:
            return "Offline";
            break;

        case EOnline:
            return "Online";
            break;

        case EAway:
            return "Away";
            break;

        case ENotAvailable:
            return "Not Available";
            break;

        case EDoNotDisturb:
            return "Do Not Disturb";
            break;

        case EInvisible:
             return "Invisible";
             break;

        case ESkypeMe:
            return "SkypeMe";
            break;

/* Effectively the same as EUnknown */
        default:
            return "Unknown";
            break;
    }
}

QString MainWindow::LoadHttpFeed(QString aHttpUri) {

    QNetworkAccessManager *netArbitrator = new QNetworkAccessManager(this);
    QObject::connect(netArbitrator, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url(aHttpUri);
    QNetworkReply *reply = netArbitrator->get(QNetworkRequest(QUrl(url)));


    qDebug() << "Data from Network: " << QString(iNetworkData);

    if (aHttpUri.startsWith("http://mystatus.skype.com")) {
        QString processedUri(aHttpUri.remove("http://mystatus.skype.com/").remove(".num"));
        qDebug() << "Got a Skype status URL" << (processedUri);
        iSkypeCache.insert(processedUri, iNetworkData);

        return iSkypeCache.value(processedUri);
    }

    if (aHttpUri.startsWith("http://api.twitter.com/1/users/show.json?id=")) {
        QString processedUri(aHttpUri.remove("http://api.twitter.com/1/users/show.json?id="));
        qDebug() << "Got a Twitter user URL" << processedUri;

        if (!iTwitterCache.value(processedUri).contains(iNetworkData)) {

        iTwitterCache.insert(processedUri, iNetworkData);

        qDebug() << "This user's cached feed data: " << iTwitterCache.value(processedUri);
        qDebug() << "Cache contains"
                 << QString::number(iTwitterCache.size())
                 << "entries for: "
                 << iTwitterCache.keys();

        return iTwitterCache.value(processedUri);

        }

    }


    else {
        return iNetworkData;
    }

    reply->close();
}

void MainWindow::finishedSlot(QNetworkReply* aReply) {
    iNetworkData = QString(aReply->readAll().data());
    qDebug() << iNetworkData;
    aReply->close();
}

void MainWindow::on_actionUpdate_Twitter_Feeds_triggered()
{

    ui->webView->setUrl(QUrl(iContentType +
                             iStartRender +
                             BuildStatusItem(GetTwitterLatestTweet(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=vmlemon")), GetTwitterAvatarUri(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=vmlemon")), "E9F09C") +
                             //BuildStatusItem(GetTwitterLatestTweet(LoadDiskFeed("../CodeTests/wtroberts.json")), GetTwitterAvatarUri(LoadDiskFeed("../CodeTests/wtroberts.json")), "EDCACA") +
                             BuildStatusItem(GetTwitterLatestTweet(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=hideout")), GetTwitterAvatarUri(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=hideout")), "CAD2ED") +
                             //BuildStatusItem(GetTwitterLatestTweet(LoadDiskFeed("../CodeTests/__MarkW__.json")), GetTwitterAvatarUri(LoadDiskFeed("../CodeTests/__MarkW__.json")), "D3F5D5") +
                             BuildStatusItem(GetTwitterLatestTweet(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=pjwaffle")), GetTwitterAvatarUri(LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=pjwaffle")), "FFC6A1") +
                             iEndRender));
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    ui->webView->show();
    qDebug() << ui->webView->url().toString();
}

void MainWindow::on_actionGet_Skype_Status_triggered()
{
   ui->SkypeStatus->setText(ParseSkypeStatus(LoadHttpFeed("http://mystatus.skype.com/" + ui->lineEdit->text() + ".num")));
}
