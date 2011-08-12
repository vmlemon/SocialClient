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
    QString GetSurname();
    QString GetEMailAddress();
    QString GetTwitterUrl();
    QString GetSkypeUserName();
    int GetSkypeStatus();
    QString GetLastFmUserName();

    /* Version 1 attributes */
    QString GetStatusColour();

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
