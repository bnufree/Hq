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
DESTDIR = ../bin/


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
    qglobalapplication.cpp \
    dbservices.cpp \
    qeastmoneychinashareexchange.cpp \
    qindexframe.cpp \
    qindexwidget.cpp \
    qeastmonystockcodesthread.cpp \
    qeastmoneyblocksharethread.cpp \
    qeastmoneyblockmangagerthread.cpp \
    qeastmoneysharefhspthread.cpp \
    qhttpget.cpp \
    qeastmoneynorthboundthread.cpp

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
    qthook.h \
    dbservices.h \
    hqdatadefines.h \
    qeastmoneychinashareexchange.h \
    qindexframe.h \
    qindexwidget.h \
    qeastmonystockcodesthread.h \
    qeastmoneyblocksharethread.h \
    qeastmoneyblockmangagerthread.h \
    qeastmoneysharefhspthread.h \
    qhttpget.h \
    qeastmoneynorthboundthread.h

FORMS    += dialog.ui \
    qexchangedatemangagedialog.ui \
    qindexframe.ui

RESOURCES += \
    image.qrc

DISTFILES += \
    qml/Components/CheckBox.qml \
    qml/Components/ComboBox.qml \
    qml/Components/DateTime.qml \
    qml/Components/DateTimeBox.qml \
    qml/Components/ImgSwitchButton.qml \
    qml/Components/LineEdit.qml \
    qml/Components/LineEditInput.qml \
    qml/Components/PushButtons.qml \
    qml/Components/ScrollBar.qml \
    qml/Components/ScrollBar1111.qml \
    qml/Components/TabButton.qml \
    qml/Components/TableWidget.qml \
    qml/Components/ToolTip.qml \
    qml/images/1.png \
    qml/images/arrow-right-black.png \
    qml/images/arrow-right-orange.png \
    qml/images/closebtn.png \
    qml/images/closebutton.png \
    qml/images/commboxarrow.png \
    qml/images/expansion.png \
    qml/images/markeicon.png \
    qml/images/right.png \
    qml/images/round-black.png \
    qml/images/round-red.png \
    qml/images/rtationblack.png \
    qml/images/rtationblack_l.png \
    qml/images/rtationwhite.png \
    qml/images/rtationwhite_l.png \
    qml/images/ship.png \
    qml/images/table_arrow.png \
    qml/AlarmDetailPanel.qml \
    qml/AlarmEventDialog.qml \
    qml/AlarmEventPage.qml \
    qml/EmergencyplanPanel.qml \
    qml/ExtrapolationSettings.qml \
    qml/FrameSelection.qml \
    qml/HistoryTrackShow.qml \
    qml/InformationPanel.qml \
    qml/MarkeLocation.qml \
    qml/MarkeLocationPanel.qml \
    qml/MessageInfo.qml \
    qml/MessageManage.qml \
    qml/MeteorologyDetail.qml \
    qml/OnlineMessagePage.qml \
    qml/RadarAfterglowSetting.qml \
    qml/RadarFusionSplit.qml \
    qml/RadarInformationLists.qml \
    qml/RadarListPage.qml \
    qml/ReferencePoint.qml \
    qml/RotationTool.qml \
    qml/ShipMarkPage.qml \
    qml/ShipsFileManage.qml \
    qml/SideToolPage.qml \
    qml/StatisticsData.qml \
    qml/SubmarineCableLists.qml \
    qml/SubmarineCablePanel.qml \
    qml/TidalCurveChart.qml \
    qml/ToolTipPage.qml \
    qml/StockInfomationList.qml
