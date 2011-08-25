#include "pidgin.h"
#include <QDebug>

Pidgin::Pidgin()
{
}

/*
"MIME-Version: 1.0
Content-Type: application/x-im-contact
X-IM-Protocol: msn
X-IM-Username: user@example.com
  */
QString Pidgin::GetXImContactProtocol(QByteArray aData) {
    QString contactProtocol = QString(aData);
    int index = contactProtocol.indexOf("X-IM-Protocol");

    qDebug() << "Got X-IM-Protocol" << index;
    contactProtocol.truncate(index);
    qDebug() << "Truncated to" << contactProtocol;

    return contactProtocol;

}

QString Pidgin::GetXImContactUsername(QByteArray aData) {
    QString contactUsername = QString(aData);

    qDebug() << "Got X-IM-Username" << contactUsername.indexOf("X-IM-Username");

    return contactUsername;

}
