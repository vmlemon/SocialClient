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

protected:
    int iVersion;
    qint64 iUid;
    QString iForename;
    QString iSurname;
    QString iEMailAddress;
    QString iTwitterUrl;
    QString iSkypeUserName;
    int iSkypeStatus;
    QString iLastFmUserName;
};

#endif // CONTACT_H
