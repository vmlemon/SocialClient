#include "contact.h"
#include <QVariant>
#include <QVariantMap>
#include <QByteArray>
#include <QMap>

#include <QtJSON/json.h>

Contact::Contact():

    /* Version 0 attributes */

    iVersion(1),
    iUid(0),
    iForename(""),
    iSurname(""),
    iEMailAddress(""),
    iTwitterUrl(""),
    iSkypeUserName(""),
    iSkypeStatus(0),
    iLastFmUserName(""),

    /* Version 1 attributes */

    iStatusColour("")

{
}

Contact::Contact(QString aJsonData){

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();

    /* Version 0 attributes */

    iVersion = dataMap["Version"].toInt();
    iUid = dataMap["Uid"].toULongLong();
    iForename = dataMap["Forename"].toString();
    iSurname = dataMap["Surname"].toString();
    iEMailAddress = dataMap["EmailAddress"].toString();
    iTwitterUrl = dataMap["TwitterUrl"].toString();
    iSkypeUserName = dataMap["SkypeUserName"].toString();
    iSkypeStatus = dataMap["SkypeStatus"].toInt();
    iLastFmUserName = dataMap["LastFmUserName"].toString();

    if (iVersion == 1) {
        iStatusColour = dataMap["StatusColour"].toString();
    }
}

QString Contact::Serialise() {
    QVariantMap contactMap;
    QString contactObject;

    /* Version 0 attributes */

    contactMap["Version"] = iVersion;
    contactMap["Uid"] = iUid;
    contactMap["Forename"] = iForename;
    contactMap["Surname"] = iSurname;
    contactMap["EmailAddress"] = iEMailAddress;
    contactMap["TwitterUrl"] = iTwitterUrl;
    contactMap["SkypeUserName"] = iSkypeUserName;
    contactMap["SkypeStatus"] = iSkypeStatus;
    contactMap["LastFmUserName"] = iLastFmUserName;

    /* Version 1 attributes */
    if (iVersion == 1) {
        contactMap["StatusColour"] = iStatusColour;
    }


    QByteArray contactArray = Json::serialize(contactMap);

    contactObject = QString::fromUtf8(contactArray);

    return contactObject;
}

/* Version 0 attributes */

int Contact::GetVersion() {
    return iVersion;
}

qint64 Contact::GetUid() {
    return iUid;
}

QString Contact::GetForename() {
    return iForename;
}

QString Contact::GetSurname() {
    return iSurname;
}

QString Contact::GetEMailAddress() {
    return iEMailAddress;
}

QString Contact::GetTwitterUrl() {
    return iTwitterUrl;
}

QString Contact::GetSkypeUserName() {
    return iSkypeUserName;
}

int Contact::GetSkypeStatus() {
    return iSkypeStatus;
}

QString Contact::GetLastFmUserName() {
    return iLastFmUserName;
}

/* Version 1 attributes */

QString Contact::GetStatusColour() {
    return iStatusColour;
}

void Contact::SetStatusColour(QString aHexColour) {
    iStatusColour = aHexColour;
}
