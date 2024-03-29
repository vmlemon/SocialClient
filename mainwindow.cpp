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
#include <contacteditor.h>

#include <contact.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    iStatusToRender("")
{
    ui->setupUi(this);

    File::DirectoryProbe(Contact::GetDefaultContactsDir());
    File::DirectoryProbe(Skype::GetDefaultCacheDir());

    on_actionUpdate_Ticker_triggered();

}

QString iContentType = "data:text/html,";
QString iUtf8ContentType = "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
QString iStartRender = "<head>" + iUtf8ContentType + "</head>" + "<body bgcolor=\"black\" text=\"white\"><marquee>";
QString iEndRender = "</marquee> &nbsp; ";

/* Username, Data */
QMap<QString, QString> iTwitterDataCache;
QMap<QString, QString> iLastFmCache;

/* UID, Username */
QMap<qint64, QString> iSkypeUidCache;
QMap<qint64, QString> iTwitterUidCache;

QString iNetworkData;

MainWindow::~MainWindow()
{
    delete ui;
}

/* API that only supports Text, Icons and Status Colour */
QString MainWindow::BuildStatusItem(QString aText, QString aIconUri, QString aStatusColour) {

    QString text = aText;
    QString iconUri = aIconUri;
    QString statusColour = aStatusColour;

    QString icon = "<img src=\"" + iconUri + "\" height=\"24\"" + "width=\"24\"/>";
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

/* API that supports Text, Icons, Status Colour, and Alternate Text (Titles) */
QString MainWindow::BuildStatusItem(QString aText, QString aIconUri, QString aStatusColour, QString aIconText) {

    QString text = aText;
    QString iconUri = aIconUri;
    QString statusColour = aStatusColour;

    QString icon = "<img src=\"" + iconUri + "\" height=\"24\"" + " title=\"" + aIconText + "\" " + "width=\"24\"/>";
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
    qDebug() << "Have" << reply->size() << "bytes";

    return iNetworkData;
}

void MainWindow::finishedSlot(QNetworkReply* aReply) {
    iNetworkData = QString(aReply->readAll().data());

    qDebug() << "Inside finishedSlot()" /* iNetworkData */;

    /*
    QSslSocket: cannot call unresolved function SSLv23_client_method
    QSslSocket: cannot call unresolved function SSL_CTX_new
    QSslSocket: cannot call unresolved function SSL_library_init
    QSslSocket: cannot call unresolved function ERR_get_error
    QSslSocket: cannot call unresolved function ERR_get_error

("https://api.twitter.com/1/users/show.json?screen_name=vmlemon")
("{"errors":[{"message":"The Twitter REST API v1 is no longer active. Please migrate to API v1.1. https://dev.twitter.com/docs/api/1.1/overview.","code":64}]}")
https://slproweb.com/products/Win32OpenSSL.html
    */

    if (aReply->url().toString().startsWith("https://api.twitter.com/1/users/show.json?id=") ||
            aReply->url().toString().startsWith("https://api.twitter.com/1/users/show.json?screen_name=") ||
            aReply->url().toString().startsWith("http://identi.ca/api/users/show.json?screen_name=")) {
        QString processedUri(Twitter::ReduceUrl(aReply->url().toString()));
        qDebug() << "Got a Twitter user URL" << processedUri;

        qDebug() << processedUri;

        iTwitterDataCache.insert(processedUri, iNetworkData);
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
        //iLastFmCache.insert(processedUri, iNetworkData);

        LastFm::WriteToCache(processedUri, iNetworkData.toUtf8());
    qDebug() << LastFm::GetLastFmLatestTrack(LastFm::ReadFromCache(processedUri));
        //qDebug() << LastFm::GetLastFmLatestTrack(iLastFmCache.value(processedUri));
    }

    aReply->close();

    ui->webView->reload();
}

void MainWindow::on_actionUpdate_Ticker_triggered()
{
    /* Not a Twitter feed, but here for testing */
    //
    /* A test of Identi.ca's "Twitter-compatible feeds" */
    //LoadHttpFeed("http://identi.ca/api/users/show.json?screen_name=identica");
    PopulateRamCache();

    QUrl workUrl = QUrl(iContentType +
                        iStartRender +
                        iStatusToRender +
                        iEndRender);


    ui->webView->setUrl(workUrl);

    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);

    ui->webView->reload();
    ui->webView->show();
    ui->webView->setUrl(workUrl);
    ui->webView->load(workUrl);

    qDebug() << "Built URL: " << workUrl;
    qDebug() << "Really using URL: " << ui->webView->url().toString();
}

void MainWindow::PopulateRamCache() {
    iStatusToRender.clear();

    QMap<qint64, QString> tweetMap;
    QMap<qint64, QString> colourMap;

    QString avatarUrl;
    QString latestTweet;
    QString colour;
    QString screenName;

    int pos = 0;

    for (pos = 0; pos < Contact::CountStoredContacts(); pos++) {
        iSkypeUidCache.insert(pos, Contact::GetSkypeUserName(pos));
        iTwitterUidCache.insert(pos, Twitter::ReduceUrl(Contact::GetTwitterUrl(pos)));
        LoadHttpFeed("https://api.twitter.com/1/users/show.json?screen_name=" +
                     Twitter::ReduceUrl(Contact::GetTwitterUrl(pos)));

        if (Contact::GetLastFmUserName(pos).length() != 0) {
            LoadHttpFeed("http://ws.audioscrobbler.com/1.0/user/" +
                         Contact::GetLastFmUserName(pos) + "/recenttracks.xml?limit=1");
        }

        tweetMap.insert(pos, iTwitterDataCache.value(iTwitterUidCache.value(pos)));

        qDebug() << tweetMap;
        colourMap.insert(pos, Contact::GetStatusColour(pos));

        avatarUrl = Twitter::GetTwitterAvatarUrl(iTwitterDataCache.value(Contact::GetTwitterUrl(pos)));
        latestTweet = Twitter::GetTwitterLatestTweet(tweetMap.value(pos));
        colour = Contact::GetStatusColour(pos);
        screenName = Twitter::GetTwitterScreenName(tweetMap.value(pos));

        qDebug() << Twitter::GetTwitterPrivacyState(iTwitterDataCache.value(Contact::GetTwitterUrl(pos)));

        iStatusToRender.append(BuildStatusItem(latestTweet,avatarUrl,colour,screenName));

        /* Attempt to set Skype statuses for UIDs */
        if (iSkypeUidCache.value(pos).length() != 0) {
            LoadHttpFeed("http://mystatus.skype.com/" + iSkypeUidCache.value(pos) + ".num");

            Contact *su = new Contact();
            su->ReadContactFile(pos);

            qDebug() << "User's status:" << Skype::ReadFromCache(Contact::GetSkypeUserName(pos));
            su->SetSkypeStatus(Skype::ReadFromCache(iSkypeUidCache.value(pos)).toInt());
            su->WriteContactFile();
            qDebug() << "Attempted to set Skype status to" << Skype::ReadFromCache(iSkypeUidCache.value(pos)).toInt();
        }


    }

    qDebug() << iSkypeUidCache;
    qDebug() << "Twitter UIDs in cache: " <<iTwitterUidCache << "Usernames: " << iTwitterDataCache.keys();

    qDebug() << "The cache contains " << QString::number(iTwitterDataCache.size()) << "items";
    qDebug() << iTwitterDataCache.keys();
    qDebug() << iTwitterDataCache.values();
    qDebug() << iLastFmCache.values();

}

void MainWindow::on_actionContact_Viewer_triggered()
{
    ContactEditor *ce = new ContactEditor();
    ce->show();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionContactManager_triggered()
{
    ContactBuilder *cb = new ContactBuilder(0, 0);
    cb->show();
}
