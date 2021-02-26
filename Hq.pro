#-------------------------------------------------
#
# Project created by QtCreator 2015-11-07T14:01:03
#
#-------------------------------------------------

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=$${PWD}/QtXlsx         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=$${QXLSX_PARENTPATH}/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=$${QXLSX_PARENTPATH}/source/  # current QXlsx source path is ./source/
message("qt xlsx path:" + $${QXLSX_PARENTPATH})
include($${QXLSX_PARENTPATH}/QXlsx.pri)

QT       += core gui network sql concurrent
android {
 QT+= androidextras
 }

#win32{
# QT += webkitwidgets
#}

CONFIG += c++11

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
    qshareforeignvolchangecounterthread.cpp \
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
    utils/networkcookie.cpp \
    qcontainerwidget.cpp \
    real/qsinanorthrealinfothread.cpp \
    real/qnorthflowinfodisplaywidget.cpp \
    history/sharehistoryreadthread.cpp \
    history/qforignvolcheckdlg.cpp \
    table/qshareforeignvoltablewidget.cpp \
    utils/hqinfoparseutil.cpp \
    qsahreoptwidget.cpp \
    real/qhqindexthread.cpp \
    zchxdisplaywidget.cpp \
    zchxmainwindow.cpp \
    real/qhqeastmoneyrealinfothread.cpp \
    real/qhqsysteminfowidget.cpp \
    real/hqkuaixun.cpp \
    qkuaixunlistwidget.cpp \
    qhqdisplaywidget.cpp \
    table/zchxhqdisplaywidget.cpp \
    qpopwindow.cpp \
    colortextlabel.cpp \
    qeastmoneyshareholderfetcher.cpp \
    table/qshareexchangetable.cpp \
    table/northboundmoneyflowtablewidget.cpp \
    table/northboundtop10displaywidget.cpp \
    table/northboundsharevolwidget.cpp \
    qetfscalethread.cpp \
    northbundwidget.cpp \
    date/annualcloseddaydisclosure.cpp \
    date/qshareactivedateupdatethread.cpp \
    date/shareworkingdatetime.cpp \
    history/qsharehsgthistorychangethread.cpp \
    qshareexchangedisplaywidget.cpp \
    table/HqMergeTableWidget.cpp

HEADERS  += dialog.h \
    qeastmoneylhbthread.h \
    qglobalapplication.h \
#    qthook.h \
    qindexframe.h \
    qindexwidget.h \
    history/qsharehistoryinfomgr.h \
#    webfile/debugout.h \
#    webfile/weberror.h \
#    webfile/webfile.h \
    exchange/qexchangerecordworker.h \
    exchange/qexchangefiledlg.h \
    qshareforeignvolchangecounterthread.h \
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
    dbservices/hqdatadefines.h \     \
    zchxdisplaywidget.h \
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
    utils/networkcookie.h \
    qcontainerwidget.h \
    real/qsinanorthrealinfothread.h \
    real/qnorthflowinfodisplaywidget.h \
    history/sharehistoryreadthread.h \
    history/qforignvolcheckdlg.h \
    table/qshareforeignvoltablewidget.h \
    utils/hqinfoparseutil.h \
    qsahreoptwidget.h \
    real/qhqindexthread.h \
    zchxmainwindow.h \
    real/qhqeastmoneyrealinfothread.h \
    real/qhqsysteminfowidget.h \
    real/hqkuaixun.h \
    qkuaixunlistwidget.h \
    qhqdisplaywidget.h \
    table/zchxhqdisplaywidget.h \
    qpopwindow.h \
    colortextlabel.h \
    qeastmoneyshareholderfetcher.h \
    table/qshareexchangetable.h \
    table/northboundmoneyflowtablewidget.h \
    table/northboundtop10displaywidget.h \
    table/northboundsharevolwidget.h \
    qetfscalethread.h \
    northbundwidget.h\
    date/annualcloseddaydisclosure.h \
    date/qshareactivedateupdatethread.h \
    date/shareworkingdatetime.h \
    history/qsharehsgthistorychangethread.h \
    qshareexchangedisplaywidget.h \
    table/HqMergeTableWidget.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui \
    exchange/qexchangefiledlg.ui \
    history/qsharehistorydialog.ui \
    qdatamgrwidget.ui \
    qsharecodenamelable.ui \
    real/qnorthflowinfodisplaywidget.ui \
    history/qforignvolcheckdlg.ui \
    qsahreoptwidget.ui \
    zchxmainwindow.ui \
    real/qhqsysteminfowidget.ui \
    qkuaixunlistwidget.ui \
    table/zchxhqdisplaywidget.ui \
    qpopwindow.ui \
    qshareexchangedisplaywidget.ui

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

ANDROID_ABIS = armeabi-v7a

ANDROID_EXTRA_LIBS =

contains(ANDROID_TARGET_ARCH,) {
    ANDROID_EXTRA_LIBS =

    ANDROID_ABIS = \
        armeabi-v7a \
        x86
}
