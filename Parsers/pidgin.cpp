#include "pidgin.h"

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
    QString contactProtocol;

    return contactProtocol;

}

QString Pidgin::GetXImContactUsername(QByteArray aData) {
    QString contactUsername;

    return contactUsername;

}
