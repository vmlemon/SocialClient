#ifndef TWITTER_H
#define TWITTER_H

#include <QString>

class Twitter
{
public:
    Twitter();
    static QString GetTwitterAvatarUrl(QString aJsonData);
    static QString GetTwitterLatestTweet(QString aJsonData);
    static QString ReduceUrl(QString aUrl);
    static QString GetTwitterScreenName(QString aJsonData);
};

#endif // TWITTER_H
