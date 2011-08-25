#include "pidgin.h"
#include <QStringList>

Pidgin::Pidgin()
{
}

QString Pidgin::GetXImContactProtocol(QByteArray aData) {
    QString contactProtocol = QString(aData);
    QStringList atoms = contactProtocol.split("\n");
    QString magic = "X-IM-Protocol: ";

    return atoms.filter(magic).first().remove(magic).simplified();

}

QString Pidgin::GetXImContactUsername(QByteArray aData) {
    QString contactUsername = QString(aData);
    QStringList atoms = contactUsername.split("\n");
    QString magic = "X-IM-Username: ";

    return atoms.filter(magic).first().remove(magic).simplified();

}
