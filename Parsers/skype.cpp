#include "skype.h"
#include <QDebug>

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
