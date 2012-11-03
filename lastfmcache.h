#ifndef LASTFMCACHE_H
#define LASTFMCACHE_H

#include <QString>

class LastFmCache
{
public:
    LastFmCache();
    static QString GetDefaultCacheDir();
    static void WriteToCache(QString aUsername, QString aData);
    static QString ReadFromCache(QString aUsername);
};

#endif // LASTFMCACHE_H
