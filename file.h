#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFile>

class File
{
public:
    File();
    static QString LoadDiskFile(QString aFilePath);
    static bool SaveDiskFile(QString aFilePath, QString aFileData);
};

#endif // FILE_H
