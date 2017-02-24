#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T14:01:03
#
#-------------------------------------------------

QT       += core gui network sql concurrent

//DEFINES *= Q_QDOC

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hq
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    stockdata.cpp \
    dataexchange.cpp \
    stockdatathread.cpp \
    blockdata.cpp \
    basedata.cpp \
    basehttpget.cpp \
    stockhttpget.cpp \
    blockhttpget.cpp \
    qeastmoneyblockthread.cpp \
    profiles.cpp \
    qsinastkinfothread.cpp \
    qsinasearchthread.cpp \
    qsinastkresultmergethread.cpp \
    qeastmoneylhbthread.cpp \
    qeastmoneyzjlxthread.cpp \
    stkmktcap.cpp \
    qeastmoneystockinfothread.cpp \
    stkinfofilemanage.cpp \
    qexchangedatamanage.cpp \
    qexchangedatemangagedialog.cpp \
    qglobalapplication.cpp

HEADERS  += dialog.h \
    stockdata.h \
    dataexchange.h \
    stockdatathread.h \
    blockdata.h \
    basedata.h \
    basehttpget.h \
    stockhttpget.h \
    blockhttpget.h \
    qeastmoneyblockthread.h \
    profiles.h \
    qsinastkinfothread.h \
    qsinasearchthread.h \
    qsinastkresultmergethread.h \
    qeastmoneylhbthread.h \
    qeastmoneyzjlxthread.h \
    stkmktcap.h \
    qeastmoneystockinfothread.h \
    stkinfofilemanage.h \
    qexchangedatamanage.h \
    qexchangedatemangagedialog.h \
    qglobalapplication.h \
    qthook.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui

RESOURCES += \
    image.qrc
