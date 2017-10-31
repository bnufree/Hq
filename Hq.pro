#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T14:01:03
#
#-------------------------------------------------

include(./QtXlsx/QtXlsx.pri)
QT       += core gui network sql concurrent

#DEFINES *= Q_QDOC

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hq
TEMPLATE = app
DESTDIR = ../bin/

SOURCES += main.cpp\
        dialog.cpp \
    stockdata.cpp \
    dataexchange.cpp \
    profiles.cpp \
    qsinastkinfothread.cpp \
    qsinasearchthread.cpp \
    qsinastkresultmergethread.cpp \
    qeastmoneylhbthread.cpp \
    qeastmoneyzjlxthread.cpp \
    stkmktcap.cpp \
    stkinfofilemanage.cpp \
    qexchangedatamanage.cpp \
    qexchangedatemangagedialog.cpp \
    qglobalapplication.cpp \
    qeastmoneychinashareexchange.cpp \
    qindexframe.cpp \
    qindexwidget.cpp \
    qeastmonystockcodesthread.cpp \
    qeastmoneysharefhspthread.cpp \
    qhttpget.cpp \
    qeastmoneynorthboundthread.cpp \
    qeastmoneyhsgtdialog.cpp \
    history/qeastmoneystockhistoryinfothread.cpp \
    history/qsharehistoryinfomgr.cpp \
    webfile/weberror.cpp \
    webfile/webfile.cpp \
    exchange/qexchangerecordworker.cpp \
    exchange/qexchangefiledlg.cpp \
    qeastmoneyhsgtshareamount.cpp \
    table/qstktablewidgetitem.cpp \
    table/HqTableWidget.cpp \
    table/qsharetablewidget.cpp \
    block/blockdata.cpp \
    block/qeastmoneyblockmangagerthread.cpp \
    block/qeastmoneyblocksharethread.cpp \
    block/qeastmoneyblockthread.cpp \
    table/qblocktablewidget.cpp \
    dbservices/hqdbdatabase.cpp \
    dbservices/dbservices.cpp

HEADERS  += dialog.h \
    stockdata.h \
    dataexchange.h \
    profiles.h \
    qsinastkinfothread.h \
    qsinasearchthread.h \
    qsinastkresultmergethread.h \
    qeastmoneylhbthread.h \
    qeastmoneyzjlxthread.h \
    stkmktcap.h \
    stkinfofilemanage.h \
    qexchangedatamanage.h \
    qexchangedatemangagedialog.h \
    qglobalapplication.h \
    qthook.h \
    dbservices.h \
    qeastmoneychinashareexchange.h \
    qindexframe.h \
    qindexwidget.h \
    qeastmonystockcodesthread.h \
    qeastmoneysharefhspthread.h \
    qhttpget.h \
    qeastmoneynorthboundthread.h \
    qeastmoneyhsgtdialog.h \
    history/qeastmoneystockhistoryinfothread.h \
    history/qsharehistoryinfomgr.h \
    webfile/debugout.h \
    webfile/weberror.h \
    webfile/webfile.h \
    exchange/qexchangerecordworker.h \
    exchange/qexchangefiledlg.h \
    qeastmoneyhsgtshareamount.h \
    table/qstktablewidgetitem.h \
    utils/comdatadefines.h \
    table/HqTableWidget.h \
    table/qsharetablewidget.h \
    block/blockdata.h \
    block/qeastmoneyblockmangagerthread.h \
    block/qeastmoneyblocksharethread.h \
    block/qeastmoneyblockthread.h \
    table/qblocktablewidget.h \
    dbservices/hqdbdatabase.h \
    dbservices/dbservices.h \
    dbservices/hqdatadefines.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui \
    qeastmoneyhsgtdialog.ui \
    exchange/qexchangefiledlg.ui

RESOURCES += \
    image.qrc
