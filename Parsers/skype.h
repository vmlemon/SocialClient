#ifndef SKYPE_H
#define SKYPE_H

#include <QString>
#include <QVariantMap>
#include <QByteArray>

enum TSkypeStatus {
    EUnknown = 0,
    EOffline = 1,
    EOnline = 2,
    EAway = 3,
    ENotAvailable = 4,
    EDoNotDisturb = 5,
    EInvisible = 6,
    ESkypeMe = 7
};

class Skype
{
public:
    Skype();
    static int GetRawSkypeStatus (QString aStatusData);
    static QString ParseSkypeStatus(QString aStatusData);

    /* Status colours */
    static QString GetOnlineColour();
    static QString GetOfflineColour();
    static QString GetUnknownColour();

    /* Cache */
    static void WriteToCache(QString aUsername, QString aData);
    static QString ReadFromCache(QString aUsername);
    static QString GetStatusColour(QString aUsername);
    static QString GetDefaultCacheDir();
    static void DirectoryProbe();

    /* Internal data formats */
    static QString ParseClipboardData(QByteArray aRawData);

};

#endif // SKYPE_H
