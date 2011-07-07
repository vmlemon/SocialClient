#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebView>
#include <QWebFrame>
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QBuffer>
#include <QFile>
#include <QMap>
#include <QtXml>

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
QMap<QString, QString> iLastFmCache;

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

     return iNetworkData;
    }

void MainWindow::finishedSlot(QNetworkReply* aReply) {
    iNetworkData = QString(aReply->readAll().data());

    qDebug() << "Inside finishedSlot()" /* iNetworkData */;


    if (aReply->url().toString().startsWith("http://api.twitter.com/1/users/show.json?id=") ||
        aReply->url().toString().startsWith("http://api.twitter.com/1/users/show.json?screen_name=")) {
        QString processedUri(aReply->url().toString().remove("http://api.twitter.com/1/users/show.json?id=")
                             .remove("http://api.twitter.com/1/users/show.json?screen_name="));
        qDebug() << "Got a Twitter user URL" << processedUri;

        qDebug() << aReply->url().toString();

        iTwitterCache.insert(processedUri, iNetworkData);

    }

    if (aReply->url().toString().startsWith("http://mystatus.skype.com")) {
        QString processedUri(aReply->url().toString().remove("http://mystatus.skype.com/").remove(".num"));
        qDebug() << "Got a Skype status URL" << (processedUri);
        iSkypeCache.insert(processedUri, iNetworkData);
    }

    if (aReply->url().toString().startsWith("http://ws.audioscrobbler.com/1.0/user/") &&
            aReply->url().toString().contains("recenttracks.xml")) {
        QString processedUri(aReply->url().toString().remove("http://ws.audioscrobbler.com/1.0/user/")
                             .remove("/recenttracks.xml").remove("?limit=1"));
        qDebug() << "Got a Last.FM Recent Tracks (API v1.0) URL" << (processedUri);
        iLastFmCache.insert(processedUri, iNetworkData);
    }

    aReply->close();
}

void MainWindow::on_actionUpdate_Twitter_Feeds_triggered()
{
    BuildTwitterCache();

}

void MainWindow::on_actionGet_Skype_Status_triggered()
{
   ui->SkypeStatus->setText(ParseSkypeStatus(LoadHttpFeed("http://mystatus.skype.com/" + ui->lineEdit->text() + ".num")));
}

void MainWindow::BuildTwitterCache() {
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=vmlemon");
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=hideout");
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=pjwaffle");
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?screen_name=9600");

    /* Not a Twitter feed, but here for testing */
    LoadHttpFeed("http://ws.audioscrobbler.com/1.0/user/vmlemon/recenttracks.xml?limit=1");

    qDebug() << "The cache contains " << QString::number(iTwitterCache.size()) << "items";
    qDebug() << iTwitterCache.keys();
    qDebug() << iTwitterCache.values();

    qDebug() << iLastFmCache.values();

    qDebug() << GetLastFmLatestTrack(iLastFmCache.value("vmlemon"));

}

void MainWindow::on_actionUpdate_Ticker_triggered()
{

    ui->webView->setUrl(QUrl(iContentType +
                             iStartRender +

                             BuildStatusItem(GetTwitterLatestTweet(iTwitterCache.value("vmlemon")), GetTwitterAvatarUri(iTwitterCache.value("vmlemon")), "E9F09C") +
                             BuildStatusItem(GetTwitterLatestTweet(LoadDiskFeed("../CodeTests/wtroberts.json")), GetTwitterAvatarUri(LoadDiskFeed("../CodeTests/wtroberts.json")), "EDCACA") +
                             BuildStatusItem(GetTwitterLatestTweet(iTwitterCache.value("hideout")), GetTwitterAvatarUri(iTwitterCache.value("hideout")), "CAD2ED") +
                             BuildStatusItem(GetTwitterLatestTweet(LoadDiskFeed("../CodeTests/__MarkW__.json")), GetTwitterAvatarUri(LoadDiskFeed("../CodeTests/__MarkW__.json")), "D3F5D5") +
                             BuildStatusItem(GetTwitterLatestTweet(iTwitterCache.value("pjwaffle")), GetTwitterAvatarUri(iTwitterCache.value("pjwaffle")), "FFC6A1") +
                             BuildStatusItem(GetTwitterLatestTweet(iTwitterCache.value("9600")), GetTwitterAvatarUri(iTwitterCache.value("9600")), "E199F0") +
                             iEndRender));
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    ui->webView->show();
    qDebug() << ui->webView->url().toString();
}

QString MainWindow::GetLastFmLatestTrack(QString aXmlData) {

    QString workingPayload = aXmlData.remove("</track>").remove("</recenttracks>").simplified();
    QString artistElement;
    QString nameElement;

    int artistOffset = aXmlData.indexOf("<artist");
    int nameOffset = workingPayload.indexOf("<name>");

    /* Strip out MusicBrainz metadata */
    workingPayload.remove(QRegExp("<mbid>*</mbid>", Qt::CaseInsensitive, QRegExp::Wildcard));

    if (aXmlData.length() !=0) {

        /* Strip away everything before <artist> */

        qDebug() << "Found the <artist> element at" << QString::number(artistOffset);
        qDebug() << "Stripping lines prior to <artist>";
        workingPayload = aXmlData.remove(0, artistOffset);
        qDebug() << workingPayload;
        artistOffset = workingPayload.indexOf("<artist");
        qDebug() << "<artist> offset has moved to" << QString::number(artistOffset);

        /* Quick-and-dirty name fetching */
        nameOffset = workingPayload.indexOf("<name>");

        qDebug() << "<name> is at" << QString::number(nameOffset);
    }

qDebug() << aXmlData;
return "nothing to see";
}
