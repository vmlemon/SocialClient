#ifndef LASTFM_H
#define LASTFM_H

#include <QString>

class LastFm
{
public:
    LastFm();
    static QString GetLastFmLatestTrack(QString aXmlData);
    static QString GetLastFmLatestTrack(QString aXmlData, QString aFormat);
};

#endif // LASTFM_H
