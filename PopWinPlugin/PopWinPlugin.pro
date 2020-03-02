#-------------------------------------------------
#
# Project created by QtCreator 2020-02-29T11:55:51
#
#-------------------------------------------------

QT       += core gui network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PopWinPlugin
TEMPLATE = app


SOURCES += main.cpp\
        qpopwindow.cpp \
    hqkuaixun.cpp \
    networkcookie.cpp \
    qhttpget.cpp

HEADERS  += qpopwindow.h \
    hqkuaixun.h \
    networkcookie.h \
    qhttpget.h

FORMS    += qpopwindow.ui
