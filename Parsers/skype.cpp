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
    qDebug() << "[Skype] Skype cache contains:" << dataMap["Data"];

    return QString::number(dataMap["Data"].toInt());
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

QString Skype::ParseClipboardData(QByteArray aRawData) {
    QByteArray workingData = aRawData;
    int stringSize = aRawData.at(0);
    int pos = 0;

    QString data;

    qDebug() << "Got data:" << aRawData.toHex() << "of internal length" << QString::number(stringSize);
    qDebug() << "Size of array after filling is" << aRawData.size();

    for (pos = 0; pos < aRawData.length() - 5; pos++) {

        int size = stringSize + 1;
        //qDebug() << pos << ":" << workingData.at(pos + 2 + 2) << data;

        if (QString(workingData.at(pos)).size() != 0) {
            qDebug() << pos << ":" << workingData.at(pos + 2 + 2);
        }
    }


    qDebug() << aRawData.length() << workingData.length();

    //ushort test = 0x070000006500630068006f00310032003300;
    //For a user named "apachelogger", this returns 0c0000006100700061006300680065006c006f006700670065007200
    //For "echo123", this is 070000006500630068006f00310032003300
    //First byte is length of string, counting from 1
    //qDebug() << QString::fromUtf16(test);
    //http://forum.skype.com/index.php?showtopic=372541
    return data;
}
