#include "googlebuzz.h"
#include <QtJSON/json.h>
#include <QVariantMap>
#include <QDebug>


GoogleBuzz::GoogleBuzz()
{
}

QString GoogleBuzz::GetFeedKind(QString aJsonData) {

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    qDebug() << dataMap["data"].toString();

    return dataMap["kind"].toString();

}
