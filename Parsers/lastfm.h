#ifndef LASTFM_H
#define LASTFM_H

#include <QString>

class LastFm
{
public:
    LastFm();
    static QString GetLastFmLatestTrack(QString aXmlData);
    static QString GetLastFmLatestTrack(QString aXmlData, QString aFormat);
    static QString GetDefaultCacheDir();
    static void WriteToCache(QString aUsername, QString aData);
    static QString ReadFromCache(QString aUsername);
    static QString GetApiKey();
};

#endif // LASTFM_H
