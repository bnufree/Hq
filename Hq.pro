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
    qeastmoneylhbthread.cpp \
    qexchangedatamanage.cpp \
    qexchangedatemangagedialog.cpp \
    qglobalapplication.cpp \
    qindexframe.cpp \
    qindexwidget.cpp \
    history/qsharehistoryinfomgr.cpp \
#    webfile/weberror.cpp \
#    webfile/webfile.cpp \
    exchange/qexchangerecordworker.cpp \
    exchange/qexchangefiledlg.cpp \
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
    utils/blockdata.cpp \
    hqtaskmagrcenter.cpp \
    history/qsharehistoryinfothread.cpp \
    history/qsharehistorycounterwork.cpp \
    history/qsharehistoryfilework.cpp \
    utils/qhttpget.cpp \
    real/qeastmoneynorthboundthread.cpp \
    real/qeastmoneyzjlxthread.cpp \
    real/qsinastkinfothread.cpp \
    real/qsinastkresultmergethread.cpp \
    real/qsahrerealdisplaywidget.cpp \
    qandroidbutton.cpp \
    qdatamgrwidget.cpp \
    table/qsharemutualbundwidget.cpp \
    qandroidlistwidget.cpp

HEADERS  += dialog.h \
    dataexchange.h \
    qeastmoneylhbthread.h \
    qexchangedatamanage.h \
    qexchangedatemangagedialog.h \
    qglobalapplication.h \
#    qthook.h \
    dbservices.h \
    qindexframe.h \
    qindexwidget.h \
    history/qsharehistoryinfomgr.h \
#    webfile/debugout.h \
#    webfile/weberror.h \
#    webfile/webfile.h \
    exchange/qexchangerecordworker.h \
    exchange/qexchangefiledlg.h \
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
    utils/blockdata.h \
    hqtaskmagrcenter.h \
    history/qsharehistoryinfothread.h \
    history/qsharehistorycounterwork.h \
    history/qsharehistoryfilework.h \
    utils/qhttpget.h \
    real/qeastmoneynorthboundthread.h \
    real/qeastmoneyzjlxthread.h \
    real/qsinastkinfothread.h \
    real/qsinastkresultmergethread.h \
    real/qsahrerealdisplaywidget.h \
    qandroidbutton.h \
    qdatamgrwidget.h \
    table/qshareMutualbundwidget.h \
    qandroidlistwidget.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui \
    exchange/qexchangefiledlg.ui \
    history/qsharehistorydialog.ui \
    real/qsahrerealdisplaywidget.ui \
    qdatamgrwidget.ui

RESOURCES += \
    image.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/assets/profiles.ini

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
