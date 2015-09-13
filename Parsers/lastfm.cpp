/*
 The API endpoint was recently changed:

http://ws.audioscrobbler.com/2.0/?method=user.getRecentTracks&
  user=defsamm&api_key=<PutKeyHere>&limit=1&format=json&callback=?

  is the new way of doing things...

  http://www.samkitson.co.uk/using-json-to-access-last-fm-recently-played-tracks/

*/

#include "lastfm.h"
#include <QDebug>
#include <QVariantMap>
#include <QDir>
#include <QtJSON/json.h>
#include <file.h>

LastFm::LastFm()
{
}

QString LastFm::GetDefaultCacheDir() {
    return QDir::homePath() + "/.SocialClient/Cache/LastFM";
}


void LastFm::WriteToCache(QString aUsername, QString aData) {

    QVariantMap lfmMap;

    /* Username, Data */
    lfmMap["Username"] = aUsername;
    lfmMap["Data"] = aData.toUtf8();

    qDebug() << "Last.FM cache:" << lfmMap.keys() << lfmMap.values();

    QByteArray lfmArray = Json::serialize(lfmMap);

    qDebug() << aData;

   File::SaveDiskFile(GetDefaultCacheDir() + "/" + aUsername, lfmArray);

}

QString LastFm::ReadFromCache(QString aUsername) {

    QString jsonData = File::LoadDiskFile(GetDefaultCacheDir() + "/" + aUsername);
    bool status;

    QVariantMap dataMap = Json::parse(jsonData, status).toMap();
    qDebug() << "[lfm] lfm cache contains:" << dataMap["Data"];

    return dataMap["Data"].toString();
}


QString LastFm::GetLastFmLatestTrack(QString aXmlData) {

    return GetLastFmLatestTrack(aXmlData, " - ");
}

QString LastFm::GetLastFmLatestTrack(QString aXmlData, QString aFormat) {

    QString workingPayload = aXmlData.remove("</track>").remove("</recenttracks>").simplified();
    QString artistElement;
    QString nameElement;

    QString artistHyphenTitle;

    int artistOffset = aXmlData.indexOf("<artist");
    int nameOffset = 0;

    if (aXmlData.length() !=0) {

        /* Strip away everything before <artist> */
        artistElement = workingPayload.remove(0, artistOffset + 40);
        artistOffset = workingPayload.indexOf("<artist");

        /* Toss artistElement's content back upstream... */
        workingPayload = artistElement;

        /* Extract the artist metadata */
        artistElement.truncate(artistElement.indexOf("</artist>"));

        /* Quick-and-dirty name fetching */
        nameOffset = workingPayload.indexOf("<name>");
        nameElement = workingPayload;
        nameElement.remove(0, artistOffset + artistElement.size() + 1);
        nameElement.remove("</artist> <name>");
        nameElement.truncate(nameElement.indexOf("</name>"));

        artistHyphenTitle.append(artistElement).append(aFormat).append(nameElement);

        qDebug() << "Artist and title: " << artistHyphenTitle.toUtf8();

        return artistHyphenTitle.toUtf8();
    }

    else {
        return "No data";
    }
}
