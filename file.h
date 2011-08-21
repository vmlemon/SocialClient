#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QDir>

class File
{
public:
    File();
    static QString LoadDiskFile(QString aFilePath);
    static bool SaveDiskFile(QString aFilePath, QString aFileData);
    static void DirectoryProbe(QString aDirectoryPath) ;
};

#endif // FILE_H
