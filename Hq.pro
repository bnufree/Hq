#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T14:01:03
#
#-------------------------------------------------

#include(./QtXlsx/QtXlsx.pri)
QT       += core gui network sql concurrent

#DEFINES *= Q_QDOC

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hq
TEMPLATE = app
DESTDIR = ../bin2/

SOURCES += main.cpp\
        dialog.cpp \
    dataexchange.cpp \
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
    qindexframe.cpp \
    qindexwidget.cpp \
    qhttpget.cpp \
    qeastmoneynorthboundthread.cpp \
    qeastmoneyhsgtdialog.cpp \
    history/qeastmoneystockhistoryinfothread.cpp \
    history/qsharehistoryinfomgr.cpp \
#    webfile/weberror.cpp \
#    webfile/webfile.cpp \
    exchange/qexchangerecordworker.cpp \
    exchange/qexchangefiledlg.cpp \
    qeastmoneyhsgtshareamount.cpp \
    table/qstktablewidgetitem.cpp \
    table/HqTableWidget.cpp \
    table/qsharetablewidget.cpp \
    block/qeastmoneyblockmangagerthread.cpp \
    block/qeastmoneyblocksharethread.cpp \
    block/qeastmoneyblockthread.cpp \
    table/qblocktablewidget.cpp \
    dbservices/hqdbdatabase.cpp \
    dbservices/dbservices.cpp \
    utils/hqutils.cpp \
    history/qsharehistorydialog.cpp \
    history/qsharegraphicwidget.cpp \
    history/qhkexchangevoldataprocess.cpp \
    utils/profiles.cpp \
    basic_info/qsharebasicinfoworker.cpp \
    basic_info/qsharecodeswork.cpp \
    basic_info/qsharefhspwork.cpp \
    basic_info/qsharehsgttop10work.cpp \
    basic_info/qsharefinancialinfowork.cpp \
    utils/sharedata.cpp \
    utils/blockdata.cpp

HEADERS  += dialog.h \
    dataexchange.h \
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
#    qthook.h \
    dbservices.h \
    qindexframe.h \
    qindexwidget.h \
    qhttpget.h \
    qeastmoneynorthboundthread.h \
    qeastmoneyhsgtdialog.h \
    history/qeastmoneystockhistoryinfothread.h \
    history/qsharehistoryinfomgr.h \
#    webfile/debugout.h \
#    webfile/weberror.h \
#    webfile/webfile.h \
    exchange/qexchangerecordworker.h \
    exchange/qexchangefiledlg.h \
    qeastmoneyhsgtshareamount.h \
    table/qstktablewidgetitem.h \
    utils/comdatadefines.h \
    table/HqTableWidget.h \
    table/qsharetablewidget.h \
    block/qeastmoneyblockmangagerthread.h \
    block/qeastmoneyblocksharethread.h \
    block/qeastmoneyblockthread.h \
    table/qblocktablewidget.h \
    dbservices/hqdbdatabase.h \
    dbservices/dbservices.h \
    dbservices/hqdatadefines.h \
    utils/hqutils.h \
    history/qsharehistorydialog.h \
    history/qsharegraphicwidget.h \
    history/qhkexchangevoldataprocess.h \
    utils/profiles.h \
    basic_info/qsharebasicinfoworker.h \
    basic_info/qsharecodeswork.h \
    basic_info/qsharefhspwork.h \
    basic_info/qsharehsgttop10work.h \
    basic_info/qsharefinancialinfowork.h \
    utils/sharedata.h \
    utils/blockdata.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui \
    qeastmoneyhsgtdialog.ui \
    exchange/qexchangefiledlg.ui \
    history/qsharehistorydialog.ui

RESOURCES += \
    image.qrc
