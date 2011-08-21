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
    ui(new Ui::MainWindow),
    iStatusToRender("")
{
    ui->setupUi(this);

    File::DirectoryProbe(Contact::GetDefaultContactsDir());
    File::DirectoryProbe(Skype::GetDefaultCacheDir());

    PopulateRamCache();
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
    qDebug() << "Have" << reply->size() << "bytes";

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
    QString status = LoadHttpFeed("http://mystatus.skype.com/" + ui->lineEdit->text() + ".num");
    test->SetSkypeStatus(Skype::GetRawSkypeStatus(status));
    test->WriteContactFile();

    ui->SkypeStatus->setText(Skype::ParseSkypeStatus(status));
}

void MainWindow::BuildFeedCache() {

    /* Not a Twitter feed, but here for testing */
    LoadHttpFeed("http://ws.audioscrobbler.com/1.0/user/" + test->GetLastFmUserName() + "/recenttracks.xml?limit=1");

    /* A test of Identi.ca's "Twitter-compatible feeds" */
    //LoadHttpFeed("http://identi.ca/api/users/show.json?screen_name=identica");

        PopulateRamCache();

    qDebug() << "The cache contains " << QString::number(iTwitterDataCache.size()) << "items";
    qDebug() << iTwitterDataCache.keys();
    qDebug() << iTwitterDataCache.values();
    qDebug() << iLastFmCache.values();

}

void MainWindow::on_actionUpdate_Ticker_triggered()
{
    PopulateRamCache();
    ui->webView->setUrl(QUrl(iContentType +
                             iStartRender +
                             iStatusToRender +
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

void MainWindow::PopulateRamCache() {
    iStatusToRender.clear();

    QMap<qint64, QString> tweetMap;
    QMap<qint64, QString> colourMap;

    QString avatarUrl;
    QString latestTweet;
    QString colour;

    int pos = 0;

    for (pos = 0; pos < Contact::CountStoredContacts(); pos++) {
        iSkypeUidCache.insert(pos, Contact::GetSkypeUserName(pos));
        iTwitterUidCache.insert(pos, Twitter::ReduceUrl(Contact::GetTwitterUrl(pos)));
        LoadHttpFeed("http://api.twitter.com/1/users/show.json?screen_name=" + Twitter::ReduceUrl(Contact::GetTwitterUrl(pos)));

        tweetMap.insert(pos, iTwitterDataCache.value(iTwitterUidCache.value(pos)));
        colourMap.insert(pos, Contact::GetStatusColour(pos));

        avatarUrl = Twitter::GetTwitterAvatarUrl(iTwitterDataCache.value(Contact::GetTwitterUrl(pos)));
        latestTweet = Twitter::GetTwitterLatestTweet(tweetMap.value(pos));
        colour = Contact::GetStatusColour(pos);

        iStatusToRender.append(BuildStatusItem(latestTweet,avatarUrl,colour));

    }

    qDebug() << iSkypeUidCache;
    qDebug() << "Twitter UIDs in cache: " <<iTwitterUidCache << "Usernames: " << iTwitterDataCache.keys();
}
