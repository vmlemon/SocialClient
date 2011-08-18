#-------------------------------------------------
#
# Project created by QtCreator 2011-07-04T22:13:37
#
#-------------------------------------------------

QT       += core gui webkit network xml

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
    file.cpp

HEADERS  += mainwindow.h QtJSON/json.h \
    Parsers/lastfm.h \
    Parsers/skype.h \
    Parsers/twitter.h \
    contact.h \
    contactbuilder.h \
    file.h

FORMS    += mainwindow.ui \
    contactbuilder.ui
