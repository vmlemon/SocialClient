#include "pidgin.h"
#include <QDebug>
#include <QStringList>

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
    QStringList atoms = contactProtocol.split("\n");

    qDebug() << "Got X-IM-Protocol" << atoms;

    return atoms.at(2).remove("X-IM-Protocol: ");

}

QString Pidgin::GetXImContactUsername(QByteArray aData) {
    QString contactUsername = QString(aData);

    qDebug() << "Got X-IM-Username" << contactUsername.indexOf("X-IM-Username");

    return contactUsername;

}
