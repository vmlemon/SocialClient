#include "lastfmcache.h"

#include <QtJSON/json.h>
#include <file.h>
#include <QDebug>

LastFmCache::LastFmCache()
{
}

QString LastFmCache::GetDefaultCacheDir() {
    return QDir::homePath() + "/.SocialClient/Cache/LastFM";
}


void LastFmCache::WriteToCache(QString aUsername, QString aData) {

    QVariantMap lfmMap;

    /* Username, Data */
    lfmMap["Username"] = aUsername;
    lfmMap["Data"] = aData.toUtf8();

    qDebug() << "Last.FM cache:" << lfmMap.keys() << lfmMap.values();

    QByteArray lfmArray = Json::serialize(lfmMap);

    qDebug() << aData;

   File::SaveDiskFile(GetDefaultCacheDir() + "/" + aUsername, lfmArray);

}

QString LastFmCache::ReadFromCache(QString aUsername) {

    QString jsonData = File::LoadDiskFile(GetDefaultCacheDir() + "/" + aUsername);
    bool status;

    QVariantMap dataMap = Json::parse(jsonData, status).toMap();
    qDebug() << "[lfm] lfm cache contains:" << dataMap["Data"];

    return dataMap["Data"].toString();
}

