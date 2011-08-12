#ifndef TWITTER_H
#define TWITTER_H

#include <QString>

class Twitter
{
public:
    Twitter();
    QString GetTwitterAvatarUri(QString aJsonData);
    QString GetTwitterLatestTweet(QString aJsonData);
};

#endif // TWITTER_H
