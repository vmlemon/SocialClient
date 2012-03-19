#include "contact.h"
#include <QVariant>
#include <QVariantMap>
#include <QByteArray>
#include <QMap>
#include <QTextStream>

#include <QDebug>

#include <QtJSON/json.h>

#include <file.h>

bool iHaveZero;

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
    CountStoredContacts();
}

Contact::Contact(QString aJsonData){

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();

    /* Version 0 attributes */

    iVersion = dataMap["Version"].toInt();
    iUid = dataMap["Uid"].toULongLong();
    iForename = dataMap["Forename"].toString().toUtf8();
    iSurname = dataMap["Surname"].toString().toUtf8();
    iEMailAddress = dataMap["EmailAddress"].toString();
    iTwitterUrl = dataMap["TwitterUrl"].toString();
    iSkypeUserName = dataMap["SkypeUserName"].toString();
    iSkypeStatus = dataMap["SkypeStatus"].toInt();
    iLastFmUserName = dataMap["LastFmUserName"].toString();

    if (iVersion == 1 || iStatusColour.length() != 0) {
        qDebug() << "Got a version 1 (or later) file";
        iStatusColour = dataMap["StatusColour"].toString();
    }

    if (iStatusColour.length() == 0) {
        qDebug() << "Got a version 0 file";
        iVersion = 0;
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
    qDebug() << "iVersion says: " << iVersion;
    return iVersion;
}

void Contact::SetVersion(int aVersion) {
    qDebug() << "aVersion says: " << aVersion << "iVersion says: " << iVersion;
    iVersion = aVersion;
}

qint64 Contact::GetUid() {
    qDebug() << "GetUid() says" << iUid;
    return iUid;
}

void Contact::SetUid(qint64 aUid) {
    qDebug() << "aUid says" << aUid << "iUid says" << iUid;

    iUid = aUid;
}

QString Contact::GetForename() {
    return iForename;
}

void Contact::SetForename(QString aForename) {
    iForename = aForename;
}

QString Contact::GetSurname() {
    return iSurname;
}

void Contact::SetSurname(QString aSurname) {
    iSurname = aSurname;
}

QString Contact::GetEMailAddress() {
    return iEMailAddress;
}

void Contact::SetEMailAddress(QString aEMailAddress) {
    iEMailAddress = aEMailAddress;
}

QString Contact::GetTwitterUrl() {
    return iTwitterUrl;
}

/* Return the Twitter URL for a given UID */
QString Contact::GetTwitterUrl(int aUid) {
    Contact *ptc = new Contact();

    ptc->ReadContactFile(aUid);

    return ptc->GetTwitterUrl();

    delete ptc;
}

/* Set this instance's Twitter URL */
void Contact::SetTwitterUrl(QString aTwitterUrl) {
    iTwitterUrl = aTwitterUrl;
}

/* Return the Skype Username for a given UID */
QString Contact::GetSkypeUserName(int aUid) {
    Contact *ptc = new Contact();

    ptc->ReadContactFile(aUid);

    return ptc->GetSkypeUserName();

    delete ptc;
}

/* Get this instance's Skype Username */
QString Contact::GetSkypeUserName() {
    return iSkypeUserName;
}

/* Set this instance's Skype Username */
void Contact::SetSkypeUserName(QString aSkypeUserName) {
    iSkypeUserName = aSkypeUserName;
}

/* Get this instance's Skype Status */
int Contact::GetSkypeStatus() {
    return iSkypeStatus;
}


/* Return the Skype Status for a given UID */
int Contact::GetSkypeStatus(int aUid) {
    Contact *ptc = new Contact();

    ptc->ReadContactFile(aUid);

    return ptc->GetSkypeStatus();

    delete ptc;
}

void Contact::SetSkypeStatus(int aSkypeStatus) {
    qDebug() << "[Contact] Setting Skype status to: " << aSkypeStatus;
    iSkypeStatus = aSkypeStatus;
}

QString Contact::GetLastFmUserName() {
    return iLastFmUserName;
}

/* Return the last.fm username for a given UID */
QString Contact::GetLastFmUserName(int aUid) {
    Contact *ptc = new Contact();

    ptc->ReadContactFile(aUid);

    return ptc->GetLastFmUserName();

    delete ptc;
}

void Contact::SetLastFmUserName(QString aLastFmUserName) {
    iLastFmUserName = aLastFmUserName;
}

/* Version 1 attributes */

QString Contact::GetStatusColour() {
    return iStatusColour;
}

QString Contact::GetStatusColour(int aUid) {
    Contact *ptc = new Contact();

    ptc->ReadContactFile(aUid);

    return ptc->GetStatusColour();

    delete ptc;
}

void Contact::SetStatusColour(QString aHexColour) {
    qDebug() << "aHexColour is " << aHexColour;
    iStatusColour = aHexColour;
}

QString Contact::GetDefaultContactsDir() {
    return QDir::homePath() + "/.SocialClient/Contacts";
}

qint64 Contact::CountStoredContacts() {

    /*
      Under Windows, this always counts 2 for an "empty" directory,
       due to the fact that "." and ".." are pseudo-directories.
     */

    QDir dir = QDir(GetDefaultContactsDir());
    QFile zero(dir.path() + "/0");

    if (zero.exists()) {
        qDebug() << "0th entry exists";
        iHaveZero = true;
    }

    else {
        iHaveZero = false;
    }

    return dir.count() - 2;
}

bool Contact::WriteContactFile() {
    return File::SaveDiskFile(GetDefaultContactsDir() + "/" + QString::number(GetUid()), Serialise());
}

bool Contact::ReadContactFile (int aContactUid) {
    Contact *ptc = new Contact(File::LoadDiskFile(GetDefaultContactsDir() + "/" + QString::number(aContactUid)));
    iVersion = ptc->GetVersion();
    iUid = ptc->GetUid();
    iForename = ptc->GetForename();
    iSurname = ptc->GetSurname();
    iEMailAddress = ptc->GetEMailAddress();
    iTwitterUrl = ptc->GetTwitterUrl();
    iSkypeUserName = ptc->GetSkypeUserName();
    iSkypeStatus = ptc->GetSkypeStatus();
    iLastFmUserName = ptc->GetLastFmUserName();
    SetStatusColour(ptc->GetStatusColour());

    qDebug() << ptc->GetStatusColour();

    delete ptc;

    return true;
}

bool Contact::EntryZeroExists() {
    return iHaveZero;
}
