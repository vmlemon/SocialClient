#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebView>
#include <QWebFrame>

#include <QBuffer>
#include <QMap>
#include <file.h>

#include <Parsers/lastfm.h>
#include <Parsers/skype.h>
#include <Parsers/twitter.h>

#include <contactbuilder.h>

#include <contact.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Contact::DirectoryProbe();
    Skype::DirectoryProbe();
}

QString iContentType = "data:text/html,";
QString iUtf8ContentType = "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
QString iStartRender = "<head>" + iUtf8ContentType + "</head>" + "<body bgcolor=\"black\" text=\"white\"><marquee>";
QString iEndRender = "</marquee> &nbsp; ";

/* Username, Data */
QMap<QString, QString> iTwitterCache;
QMap<QString, QString> iLastFmCache;

QString iNetworkData;

/* Remove me later */
Contact *test;

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

void MainWindow::on_lineEdit_textChanged(const QString &aText)
{
    qDebug() << aText;
}

void MainWindow::resizeEvent(QResizeEvent *aEvent) {
    ui->webView->resize(aEvent->size().width(), ui->webView->height());
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
        aReply->url().toString().startsWith("http://api.twitter.com/1/users/show.json?screen_name=") ||
        aReply->url().toString().startsWith("http://identi.ca/api/users/show.json?screen_name=")) {
        QString processedUri(Twitter::ReduceUrl(aReply->url().toString()));
        qDebug() << "Got a Twitter user URL" << processedUri;

        qDebug() << processedUri;

        iTwitterCache.insert(processedUri, iNetworkData);

    }

    if (aReply->url().toString().startsWith("http://mystatus.skype.com")) {
        QString processedUri(aReply->url().toString().remove("http://mystatus.skype.com/").remove(".num"));
        qDebug() << "Got a Skype status URL" << (processedUri);

        Skype::WriteToCache(processedUri, iNetworkData);
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
    test = new Contact();
    test->ReadContactFile(0);

    BuildFeedCache();
}

void MainWindow::on_actionGet_Skype_Status_triggered()
{
   ui->SkypeStatus->setText(Skype::ParseSkypeStatus(LoadHttpFeed("http://mystatus.skype.com/" + ui->lineEdit->text() + ".num")));
}

void MainWindow::BuildFeedCache() {

    LoadHttpFeed(test->GetTwitterUrl());
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=hideout");
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?id=pjwaffle");
    LoadHttpFeed("http://api.twitter.com/1/users/show.json?screen_name=9600");

    /* Not a Twitter feed, but here for testing */
    LoadHttpFeed("http://ws.audioscrobbler.com/1.0/user/" + test->GetLastFmUserName() + "/recenttracks.xml?limit=1");

   /* A test of Identi.ca's "Twitter-compatible feeds" */
    LoadHttpFeed("http://identi.ca/api/users/show.json?screen_name=identica");

    qDebug() << "The cache contains " << QString::number(iTwitterCache.size()) << "items";
    qDebug() << iTwitterCache.keys();
    qDebug() << iTwitterCache.values();
    qDebug() << iLastFmCache.values();
}

void MainWindow::on_actionUpdate_Ticker_triggered()
{

    ui->webView->setUrl(QUrl(iContentType +
                             iStartRender +

                             BuildStatusItem(Twitter::GetTwitterLatestTweet(iTwitterCache.value(Twitter::ReduceUrl(test->GetTwitterUrl()))),
                                             Twitter::GetTwitterAvatarUri(iTwitterCache.value(Twitter::ReduceUrl(test->GetTwitterUrl()))),
                                                                          test->GetStatusColour()) +
                             "Listening to: " + LastFm::GetLastFmLatestTrack(iLastFmCache.value(test->GetLastFmUserName())) +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(File::LoadDiskFeed("../CodeTests/wtroberts.json")), Twitter::GetTwitterAvatarUri(File::LoadDiskFeed("../CodeTests/wtroberts.json")), "EDCACA") +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(iTwitterCache.value("hideout")), Twitter::GetTwitterAvatarUri(iTwitterCache.value("hideout")), "CAD2ED") +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(File::LoadDiskFeed("../CodeTests/__MarkW__.json")), Twitter::GetTwitterAvatarUri(File::LoadDiskFeed("../CodeTests/__MarkW__.json")), "D3F5D5") +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(iTwitterCache.value("pjwaffle")), Twitter::GetTwitterAvatarUri(iTwitterCache.value("pjwaffle")), "FFC6A1") +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(iTwitterCache.value("9600")), Twitter::GetTwitterAvatarUri(iTwitterCache.value("9600")), "E199F0") +
                             BuildStatusItem(Twitter::GetTwitterLatestTweet(iTwitterCache.value("identica")), Twitter::GetTwitterAvatarUri(iTwitterCache.value("identica")), "F1C6F5") +
                             iEndRender));
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
    ui->webView->show();
    qDebug() << ui->webView->url().toString();

    ui->SkypeStatus->setText(Skype::ParseSkypeStatus(QString::number(test->GetSkypeStatus())));

}

void MainWindow::on_actionCreate_Contact_triggered()
{
    ContactBuilder *cb = new ContactBuilder();
    cb->show();
}
