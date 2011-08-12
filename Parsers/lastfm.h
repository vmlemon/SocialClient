#ifndef LASTFM_H
#define LASTFM_H

#include <QString>

class LastFm
{
public:
    LastFm();
    QString GetLastFmLatestTrack(QString aXmlData);
};

#endif // LASTFM_H
