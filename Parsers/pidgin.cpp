#include "pidgin.h"
#include <QStringList>

Pidgin::Pidgin()
{
}

QString Pidgin::GetXImContactProtocol(QByteArray aData) {
    return QString(aData).split("\n").filter("X-IM-Protocol: ").first().remove("X-IM-Protocol: ").simplified();
}

QString Pidgin::GetXImContactUsername(QByteArray aData) {
    return QString(aData).split("\n").filter("X-IM-Username: ").first().remove("X-IM-Username: ").simplified();
}
