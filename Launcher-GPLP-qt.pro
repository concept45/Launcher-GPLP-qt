#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T22:22:09
#
#-------------------------------------------------

QT       += core gui sql network

# CONFIG   += c++11

QMAKE_CXXFLAGS += -std=c++11 -D__NO_INLINE__

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Launcher-GPLP-qt
TEMPLATE = app


SOURCES += main.cpp\
        launchermain.cpp \
    gdialog.cpp \
    commands.cpp \
    commandshandler.cpp \
    filedownloader.cpp

HEADERS  += launchermain.h \
    types.h \
    gdialog.h \
    commands.h \
    filedownloader.h

FORMS    += launchermain.ui

RESOURCES += \
    resourcefiles.qrc
