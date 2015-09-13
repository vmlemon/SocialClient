#-------------------------------------------------
#
# Project created by QtCreator 2011-07-04T22:13:37
#
#-------------------------------------------------

QT       += core gui webkit network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = CodeTests
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        QtJSON/json.cpp \
    Parsers/lastfm.cpp \
    Parsers/skype.cpp \
    Parsers/twitter.cpp \
    contact.cpp \
    contactbuilder.cpp \
    file.cpp \
    contacteditor.cpp \
    Parsers/pidgin.cpp \
    tweetbrowser.cpp \
     o2-master\o2-master\src\o2replyserver.cpp \
     o2-master\o2-master\src\o1.cpp \
     o2-master\o2-master\src\simplecrypt.cpp \
     o2-master\o2-master\src\o2settingsstore.cpp \
     o2-master\o2-master\src\oxtwitter.cpp


HEADERS  += mainwindow.h QtJSON/json.h \
    Parsers/lastfm.h \
    Parsers/skype.h \
    Parsers/twitter.h \
    contact.h \
    contactbuilder.h \
    file.h \
    contacteditor.h \
    Parsers/pidgin.h \
    tweetbrowser.h \
    o2-master\o2-master\src\o2replyserver.h \
    o2-master\o2-master\src\o1.h \
    o2-master\o2-master\src\o1twitter.h \
    o2-master\o2-master\src\o2abstractstore.h \
    o2-master\o2-master\src\o2settingsstore.h \
     o2-master\o2-master\src\simplecrypt.h \
    o2-master\o2-master\src\oxtwitter.h

FORMS    += mainwindow.ui \
    contactbuilder.ui \
    contacteditor.ui \
    tweetbrowser.ui
