#ifndef TWITTER_H
#define TWITTER_H

#include <QString>

class Twitter
{
public:
    Twitter();
    static QString GetTwitterAvatarUri(QString aJsonData);
    static QString GetTwitterLatestTweet(QString aJsonData);
};

#endif // TWITTER_H
