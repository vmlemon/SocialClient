#include "contact.h"
#include <QVariant>
#include <QVariantMap>
#include <QByteArray>
#include <QMap>

Contact::Contact():
    iVersion(0),
    iUid(0),
    iForename(""),
    iSurname(""),
    iEMailAddress(""),
    iTwitterUrl(""),
    iSkypeUserName(""),
    iSkypeStatus(0),
    iLastFmUserName("")
{
}

Contact::Contact(QString aJsonData){

    bool status;

    QVariantMap dataMap = Json::parse(aJsonData, status).toMap();
    //qDebug() << "Avatar URI: " << dataMap["profile_image_url"].toString();

    iVersion = dataMap["Version"].toInt();
    iUid = dataMap;
    iForename("");
    iSurname("");
    iEMailAddress("");
    iTwitterUrl("");
    iSkypeUserName("");
    iSkypeStatus(0);
    iLastFmUserName("");


}

QString Contact::Serialise() {
    QVariantMap contactMap;
    QByteArray contactArray;
    QString contactObject;

    contactMap["Version"] = iVersion;
    contactMap["Uid"] = iUid;
    contactMap["Forename"] = iForename;
    contactMap["Surname"] = iSurname;
    contactMap["EmailAddress"] = iEMailAddress;
    contactMap["TwitterUrl"] = iTwitterUrl;
    contactMap["SkypeUserName"] = iSkypeUserName;
    contactMap["SkypeStatus"] = iSkypeStatus;
    contactMap["LastFmUserName"] = iLastFmUserName;

    contactArray = Json::serialize(contactMap);

    contactObject(contactArray);

    return contactObject;
}
