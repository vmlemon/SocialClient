#include "file.h"
#include <QDebug>
#include <QFile>

#include <contact.h>

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

void File::DirectoryProbe(QString aDirectoryPath) {

    QDir dir = QDir(aDirectoryPath);

    QString dirNameOrType;

    if (aDirectoryPath.contains("Skype")) {
        dirNameOrType = "Skype cache";
    }

    if (aDirectoryPath.contains("Contacts")) {
        dirNameOrType = "Contacts";
    }

    if (aDirectoryPath.contains("LastFM")) {
        dirNameOrType = "Last.FM cache";
    }

    else {
        dirNameOrType = aDirectoryPath;
    }

    if (!dir.exists()) {
        qDebug() << "Cannot locate the" << dirNameOrType << "directory. Attempting to create it...";

        dir.mkpath(aDirectoryPath);
    }

    else {
        /* Familiar formatting */
        if (aDirectoryPath.contains("Skype")) {
            qDebug() << "Will look inside" << aDirectoryPath << "for cached Skype status data";
        }

        if (aDirectoryPath.contains("LastFM")) {
            qDebug() << "Will look inside" << aDirectoryPath << "for cached Last.FM data";
        }

        if (aDirectoryPath.contains("Contacts")) {
            qDebug() << "Will look inside" << aDirectoryPath << "for contacts";
            qDebug() << "Discovered" << QString::number(Contact::CountStoredContacts()) << "contacts";
        }

    }
}

