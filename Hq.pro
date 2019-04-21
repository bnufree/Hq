#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T14:01:03
#
#-------------------------------------------------

#include(./QtXlsx/QtXlsx.pri)
QT       += core gui network sql concurrent

#DEFINES *= Q_QDOC
#DEFINES *= HQ_RUNNABLE

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hq
TEMPLATE = app
DESTDIR = ../bin3/

SOURCES += main.cpp\
        dialog.cpp \
    qeastmoneylhbthread.cpp \
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
    history/qsharehistorydialog.cpp \
    history/qsharegraphicwidget.cpp \
    history/qhkexchangevoldataprocess.cpp \
    utils/profiles.cpp \
    basic_info/qsharebasicinfoworker.cpp \
    basic_info/qsharecodeswork.cpp \
    basic_info/qsharefhspwork.cpp \
    basic_info/qsharehsgttop10work.cpp \
    basic_info/qsharefinancialinfowork.cpp \
    data_structure/sharedata.cpp \
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
    qandroidlistwidget.cpp \
    table/qsharecodenamewidget.cpp \
    dbservices/qactivedate.cpp \
    data_structure/hqutils.cpp \
    data_structure/hqbasedata.cpp \
    data_structure/hqblockdata.cpp \
    qsharecodenamelable.cpp\
    utils/networkcookie.cpp

HEADERS  += dialog.h \
    qeastmoneylhbthread.h \
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
    history/qsharehistorydialog.h \
    history/qsharegraphicwidget.h \
    history/qhkexchangevoldataprocess.h \
    utils/profiles.h \
    basic_info/qsharebasicinfoworker.h \
    basic_info/qsharecodeswork.h \
    basic_info/qsharefhspwork.h \
    basic_info/qsharehsgttop10work.h \
    basic_info/qsharefinancialinfowork.h \
    data_structure//sharedata.h \
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
    qandroidlistwidget.h \
    table/qsharecodenamewidget.h \
    dbservices/qactivedate.h \
    data_structure/hqutils.h \
    data_structure/hqbasedata.h \
    data_structure/hqblockdata.h \
    qsharecodenamelable.h\
    utils/networkcookie.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui \
    exchange/qexchangefiledlg.ui \
    history/qsharehistorydialog.ui \
    real/qsahrerealdisplaywidget.ui \
    qdatamgrwidget.ui \
    qsharecodenamelable.ui

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
