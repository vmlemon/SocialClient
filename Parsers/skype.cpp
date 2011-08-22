#include "skype.h"
#include <QDebug>
#include <QtJSON/json.h>
#include <file.h>
#include <QTextStream>

Skype::Skype()
{

}

int Skype::GetRawSkypeStatus (QString aStatusData) {
    return aStatusData.toInt();
}

QString Skype::ParseSkypeStatus(QString aStatusData) {
    qDebug() << "aStatusData contains: " << aStatusData;
    int state = aStatusData.toInt();

    switch (state) {
        case EOffline:
            return "Offline";
            break;

        case EOnline:
            return "Online";
            break;

        case EAway:
            return "Away";
            break;

        case ENotAvailable:
            return "Not Available";
            break;

        case EDoNotDisturb:
            return "Do Not Disturb";
            break;

        case EInvisible:
             return "Invisible";
             break;

        case ESkypeMe:
            return "SkypeMe";
            break;

/* Effectively the same as EUnknown */
        default:
            return "Unknown";
            break;
    }
}

QString Skype::GetOnlineColour() {
    return "#98F794"; /* Green */
}

QString Skype::GetOfflineColour() {
    return "#F7949E"; /* Pink */
}

QString Skype::GetUnknownColour() {
    return "#F7EA94"; /* Yellow */
}

void Skype::WriteToCache(QString aUsername, QString aData) {

    QVariantMap skypeMap;

    /* Username, Data */
    skypeMap["Username"] = aUsername;
    skypeMap["Data"] = aData;

    qDebug() << "Skype cache:" << skypeMap.keys() << skypeMap.values();

    QByteArray skypeArray = Json::serialize(skypeMap);

    File::SaveDiskFile(GetDefaultCacheDir() + "/" + aUsername, QString::fromUtf8(skypeArray));

}

QString Skype::ReadFromCache(QString aUsername) {

    QString jsonData = File::LoadDiskFile(GetDefaultCacheDir() + "/" + aUsername);
    bool status;

    QVariantMap dataMap = Json::parse(jsonData, status).toMap();
    qDebug() << dataMap["Data"];

    return ParseSkypeStatus(QString::number(dataMap["Data"].toInt()));
}

QString Skype::GetStatusColour(QString aUsername) {

    QString jsonData = File::LoadDiskFile(GetDefaultCacheDir() + "/" + aUsername);
    bool status;

    QVariantMap dataMap = Json::parse(jsonData, status).toMap();
    qDebug() << dataMap["Data"];

    switch (dataMap["Data"].toInt()) {
    case EOnline:
        return GetOnlineColour();

        qDebug() << "Online";

        break;

    case EOffline:
        return GetOfflineColour();

        qDebug() << "Offline";

        break;

    default:
        return GetUnknownColour();

        qDebug() << "Unknown";

        break;
    }
}


QString Skype::GetDefaultCacheDir() {
    return QDir::homePath() + "/.SocialClient/Cache/Skype";
}
