#ifndef GOOGLEBUZZ_H
#define GOOGLEBUZZ_H

#include <QString>

class GoogleBuzz
{
public:
    GoogleBuzz();
    static QString GetFeedKind(QString aJsonData);
};

#endif // GOOGLEBUZZ_H