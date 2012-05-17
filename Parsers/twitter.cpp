#include "twitter.h"
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QDebug>

Twitter::Twitter()
{
}


QString Twitter::GetTwitterScreenName(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    qDebug() << "Screen name: " << dataMap["screen_name"].toString();
    return dataMap["screen_name"].toString();
}


QString Twitter::GetTwitterAvatarUrl(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    qDebug() << "Avatar URI: " << dataMap["profile_image_url"].toString();
    return dataMap["profile_image_url"].toString();
}

QString Twitter::GetTwitterLatestTweet(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();

    QVariantMap statusMap = dataMap["status"].toMap();

    qDebug() << "Latest Tweet: " << statusMap["text"].toString();
    return statusMap["text"].toString();
}

QStringList Twitter::GetAllTweetsForUser(QString aJsonData) {
    QStringList data;

    qDebug() << aJsonData;

    data.append("Nothing yet");

    return data;
}

QString Twitter::ReduceUrl(QString aUrl) {
    return aUrl.remove("http://api.twitter.com/1/users/show.json?id=")
                         .remove("http://api.twitter.com/1/users/show.json?screen_name=")
                         .remove("http://identi.ca/api/users/show.json?screen_name=")
                         .remove("http://twitter.com/").remove("#!/");
}
