#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFile>

class File
{
public:
    File();
    static QString LoadDiskFeed(QString aFilePath);
};

#endif // FILE_H
