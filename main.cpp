#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    //This is something of a hack...
//#if QT_VERSION == 5.3.2
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 2)
        QTextCodec::setCodecForLocale(codec);
#else
        QTextCodec::setCodecForCStrings(codec);
#endif

        //QTextCodec::
    w.show();

    return a.exec();
}
