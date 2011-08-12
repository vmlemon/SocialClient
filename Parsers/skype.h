#ifndef SKYPE_H
#define SKYPE_H

#include <QString>

enum TSkypeStatus {
    EUnknown = 0,
    EOffline = 1,
    EOnline = 2,
    EAway = 3,
    ENotAvailable = 4,
    EDoNotDisturb = 5,
    EInvisible = 6,
    ESkypeMe = 7
};

class Skype
{
public:
    Skype();
    static QString ParseSkypeStatus(QString aStatusData);

private:
};

#endif // SKYPE_H
