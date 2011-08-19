#include "skype.h"
#include <QDebug>
#include <QtJSON/json.h>
#include <QDir>
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

    QFile workingFile(GetDefaultCacheDir() + "/" + aUsername);

    QVariantMap skypeMap;
    QString skypeObject;

    /* Username, Data */
    skypeMap["Username"] = aUsername;
    skypeMap["Data"] = aData;

    qDebug() << "Skype cache:" << skypeMap.keys() << skypeMap.values();

    QByteArray skypeArray = Json::serialize(skypeMap);
    skypeObject = QString::fromUtf8(skypeArray);

    workingFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&workingFile);

    stream << skypeObject;
    workingFile.close();
}

QString Skype::GetStatusColour(QString aUsername) {

    QString jsonData = File::LoadDiskFeed(GetDefaultCacheDir() + "/" + aUsername);
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

    case EUnknown:
        return GetUnknownColour();

        qDebug() << "Unknown";

        break;
    }
}

QString Skype::GetDefaultCacheDir() {
    return QDir::homePath() + "/.SocialClient/Cache/Skype";
}
