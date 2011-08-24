#ifndef PIDGIN_H
#define PIDGIN_H

#include <QByteArray>
#include <QString>

class Pidgin
{
public:
    Pidgin();
    static QString GetXImContactProtocol(QByteArray aData);
    static QString GetXImContactUsername(QByteArray aData);
};

#endif // PIDGIN_H
