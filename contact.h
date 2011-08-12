#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QtJSON/json.h>

class Contact
{
public:
    Contact();
    Contact(QString aJsonData);
    QString Serialise();

    /* Version 0 attributes */
    int GetVersion();
    qint64 GetUid();

    QString GetForename();
    void SetForename(QString aForename);

    QString GetSurname();
    void SetSurname(QString aSurname);

    QString GetEMailAddress();
    void SetEMailAddress(QString aEMailAddress);

    QString GetTwitterUrl();
    void SetTwitterUrl(QString aTwitterUrl);

    QString GetSkypeUserName();
    void SetSkypeUserName(QString aSkypeUserName);

    int GetSkypeStatus();
    void SetSkypeStatus(int aSkypeStatus);

    QString GetLastFmUserName();
    void SetLastFmUserName(QString aLastFmUserName);

    /* Version 1 attributes */
    QString GetStatusColour();
    void SetStatusColour(QString aHexColour);

protected:

    /* Version 0 attributes */
    int iVersion;
    qint64 iUid;
    QString iForename;
    QString iSurname;
    QString iEMailAddress;
    QString iTwitterUrl;
    QString iSkypeUserName;
    int iSkypeStatus;
    QString iLastFmUserName;

    /* Version 1 attributes */
    QString iStatusColour;
};

#endif // CONTACT_H
