#include "file.h"
#include <QDebug>

File::File()
{
}

QString File::LoadDiskFile(QString aFilePath) {

    QFile feedFile(aFilePath);
    QString feedLine;

    if (!feedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not read from " << aFilePath;
    }

    QTextStream feedStream(&feedFile);

    while (!feedStream.atEnd()) {
             feedLine = feedStream.readLine();
    }
    return feedLine;
}

bool File::SaveDiskFile(QString aFilePath, QString aFileData) {
    QFile workingFile(aFilePath);
    workingFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&workingFile);

    stream << aFileData;
    workingFile.close();

    if (!workingFile.exists()) {
        return false;
    }
    else {
        return true;
    }
}
