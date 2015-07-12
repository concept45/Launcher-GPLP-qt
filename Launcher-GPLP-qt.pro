#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T22:22:09
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Launcher-GPLP-qt
TEMPLATE = app


SOURCES += main.cpp\
        launchermain.cpp

HEADERS  += launchermain.h \
    types.h

FORMS    += launchermain.ui

RESOURCES += \
    resourcefiles.qrc
