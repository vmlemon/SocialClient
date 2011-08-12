#include "twitter.h"
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QDebug>

Twitter::Twitter()
{
}

QString Twitter::GetTwitterAvatarUri(QString aJsonData) {

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
