#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForCStrings(codec);

    w.show();

    return a.exec();
}
