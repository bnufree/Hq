import QtQuick 1.1
import "Components"

Rectangle {
    width: 600; height: 400; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }

    property string m_idx: "---";                 //编号
    property string m_alarmtime: "---";           //告警时间(格式:yyyy-MM-dd HH:mm:ss)
    property string m_shipnamecontent: "";        //船名预警内容
    property string m_shipnameevaluate: "";       //船名评估
    property string m_shipnamevalue: "";          //船名取值
    property string m_mmsicontent: "";            //MMSI
    property string m_mmsievaluate: "";           //MMSI
    property string m_mmsivalue: "";              //MMSI
    property string m_pointcontent: "";           //坐标
    property string m_pointevaluate: "";          //坐标
    property string m_pointvalue: "";             //坐标
    property string m_sogcontent: "";             //速度
    property string m_sogevaluate: "";            //速度
    property string m_sogvalue: "";               //速度
    property string m_locationcontent: "";        //位置
    property string m_locationevaluate: "";       //位置
    property string m_locationvalue: "";          //位置
    property string m_draughtcontent: "";         //吃水
    property string m_draughtevaluate: "";        //吃水
    property string m_draughtvalue: "";           //吃水
    property string m_anchorcontent: "";          //瞄重
    property string m_anchorevaluate: "";         //瞄重
    property string m_anchorvalue: "";            //瞄重
    property string m_scmsAcontent: "";           //A相
    property string m_scmsAevaluate: "";          //A相
    property string m_scmsAvalue: "";             //A相
    property string m_scmsBcontent: "";           //B相
    property string m_scmsBevaluate: "";          //B相
    property string m_scmsBvalue: "";             //B相
    property string m_scmsCcontent: "";           //C相
    property string m_scmsCevaluate: "";          //C相
    property string m_scmsCvalue: "";             //C相
    property string m_scmsProtectcontent: "";     //保护方式
    property string m_scmsProtectevaluate: "";     //保护方式
    property string m_scmsProtectvalue: "";         //保护方式
    property string m_scmsRealdepthcontent: "";     //埋设深度
    property string m_scmsRealDepthevaluate: "";     //埋设深度
    property string m_scmsRealDepthvalue: "";         //埋设深度
    property string m_scmsWaterDepthcontent: "";     //水深
    property string m_scmsWaterDepthevaluate: "";     //水深
    property string m_scmsWaterDepthvalue: "";         //水深
    property string m_windcontent: "";            //风速
    property string m_windevaluate: "";           //风速
    property string m_windvalue: "";              //风速
    property string m_treadcontent: "";           //潮流
    property string m_treadevaluate: "";          //潮流
    property string m_treadvalue: "";             //潮流
    property string m_tidalcontent: "";           //潮汐
    property string m_tidalevaluate: "";          //潮汐
    property string m_tidalvalue: "";             //潮汐
    property string m_voltagecontent: "";         //电压
    property string m_voltageevaluate: "";        //电压
    property string m_voltagevalue: "";           //电压
    property string m_electriccontent: "";        //电流
    property string m_electricevaluate: "";       //电流
    property string m_electricvalue: "";          //电流
    property string m_hydrauliccontent: "";       //油压
    property string m_hydraulicevaluate: "";      //油压
    property string m_hydraulicvalue: "";         //油压
    property string m_oilFlowcontent: "";         //油流
    property string m_oilFlowevaluate: "";        //油流
    property string m_oilFlowvalue: "";           //油流
    property string m_totalValues: "0.0";         //总值
    property string m_remarks: "";                //备注
    property string m_warningtype: "---";        //预警类型
    property string m_warninggrade: "---";       //预警级别

    MouseArea{ anchors.fill: parent; }
    Rectangle {
        anchors.fill: parent; color: Qt.rgba(0.113,0.145,0.168,0.8);
        Column{
            anchors{ fill: parent; margins: 2; } spacing: 0;
            /*-------------------------------------------
             * 标题
             ---------------------------------------------*/
            Item {
                id: titleItem; width: parent.width; height: 41;
                Text {
                    anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 1; }
                    font{ family: localFont.name; pointSize: 11; bold: true; }
                    color: "#ffffff"; smooth: true;
                    text: qsTr("系统预警");
                }
            }
            Rectangle{ id: lineRect; width: parent.width; height: 2; color: "#f79d00"; }
            /*-------------------------------------------
             * TAB
             ---------------------------------------------*/
            Row{
                id: buttonRow;
                width: parent.width - 83; height: 24; spacing: 0;
                ListView {
                    id: tabListView;
                    width: parent.width; height: 24;
                    model: tabListsModel;
                    delegate: tabDelegate;
                    interactive: false; clip: true;
                    orientation: ListView.Horizontal;
                    Component {
                        id: tabDelegate
                        Rectangle {
                            width: 79; height: 24; clip: true; color: "#f79d00";
                            Rectangle {
                                anchors.fill: parent; color: tabListView.currentIndex == index ? "#f79d00" : "#e5e5e5"; clip: true;
                                Text {
                                    anchors.centerIn: parent; color: "#ffffff";
                                    smooth: true; text: names;
                                    font{ family: localFont.name; pointSize: 10; bold: true; }
                                }
                            }
                            MouseArea { anchors.fill: parent; onClicked: {  tabListView.currentIndex = index;  centersview.currentIndex = index; } }
                        }
                    }
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "预警信息"; } }
                }
            }
            /*-------------------------------------------
             * 中心区域
            ---------------------------------------------*/
            ListView {
                id: centersview
                width: parent.width; height: parent.height-buttonRow.height-lineRect.height-titleItem.height;
                model: itemModel;
                currentIndex: 0; clip: true;
                preferredHighlightBegin: 0; preferredHighlightEnd: 0
                highlightRangeMode: ListView.StrictlyEnforceRange
                orientation: ListView.Horizontal;
                snapMode: ListView.SnapOneItem;
                flickDeceleration: 2000;
                interactive: false;
                onCurrentIndexChanged: { tabListView.currentIndex = centersview.currentIndex; }
                VisualItemModel {
                    id: itemModel;
                    /*-------------------------------------------
                    * 详细信息
                    ---------------------------------------------*/
                    Item{
                        width: centersview.width; height: centersview.height;
                        Column{
                            anchors.fill: parent; spacing: 0;
                            /*-------------------------------------------
                            * 详细信息
                            ---------------------------------------------*/
                            Item { width: parent.width; height: 3;}
                            Item {
                                width: parent.width; height: 22;
                                Text {
                                    anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 0; } color: "#00ceff";
                                    font{ family: localFont.name; pointSize: 11; bold: true; }
                                    smooth: true; text: qsTr("详细信息");
                                }
                            }
                            /*-------------------------------------------
                            * 线
                            ---------------------------------------------*/
                            Row{
                                width: parent.width; height: 2; spacing: -1;
                                Rectangle{ id: boldLine; width: 65; height: 3; color: "#00ceff"; radius: 2; }
                                Rectangle{ width: parent.width-boldLine.width-expansionbtn.width; height: 1; color: "#00ceff"; anchors.top: parent.bottom; }
                                BorderImage {
                                    id: expansionbtn; width: 31; height: 6;
                                    anchors.top: parent.bottom;
                                    horizontalTileMode: BorderImage.Stretch;
                                    verticalTileMode: BorderImage.Stretch;
                                    source: "qrc:/ecdisviewer/qml/images/expansion.png";
                                    smooth: true;
                                    MouseArea { anchors.fill: parent; }
                                }
                            }
                            /*-------------------------------------------
                            * 详细信息
                            ---------------------------------------------*/
                            Item{
                                width: parent.width; height: parent.height-30;
                                Flickable {
                                    anchors.fill: parent; contentHeight: 674; clip: true;
                                    Column{
                                        anchors.fill: parent; anchors.margins: 3; spacing: 2;
                                        /*-------------------------------------------
                                        * 告警时间
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26; spacing: 4;
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: qsTr("预警时间:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: m_alarmtime;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 预警类型
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 24; spacing: 4;
                                            Text {
                                                 anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: qsTr("预警类型:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: m_warningtype;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 预警级别
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 24; spacing: 4;
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: qsTr("预警级别:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                smooth: true; text: m_warninggrade;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 要素信息
                                        ---------------------------------------------*/
                                        Rectangle {
                                            width: parent.width-4; height: 499; color: Qt.rgba(1,1,1,0.02);
                                            Column{
                                                anchors.fill: parent;
                                                /*-------------------------------------------
                                                * 标题
                                                ---------------------------------------------*/
                                                Rectangle{ width:parent.width;height:1; color:"#00ceff"}
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 28; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.centerIn: parent; color: "#00ceff";
                                                            anchors.verticalCenterOffset: 2;
                                                            font{ family: localFont.name; pointSize: 10; bold: false; }
                                                            smooth: true; text: qsTr("要素");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 28; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.centerIn: parent; color: "#00ceff";
                                                            anchors.verticalCenterOffset: 2;
                                                            font{ family: localFont.name; pointSize: 10; }
                                                            smooth: true; text: qsTr("预警内容");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 28; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.centerIn: parent; color: "#00ceff";
                                                            anchors.verticalCenterOffset: 2;
                                                            font{ family: localFont.name; pointSize: 10; bold: false; }
                                                            smooth: true; text: qsTr("取值");
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 船名
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("船名");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_shipnamecontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.centerIn: parent;
                                                            anchors.verticalCenterOffset: 2;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_shipnamevalue);
                                                            visible: false;
                                                        }
                                                    }
                                                }

                                                /*-------------------------------------------
                                                * MMSI
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("MMSI");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_mmsicontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.centerIn: parent;
                                                            anchors.verticalCenterOffset: 2;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_mmsivalue);
                                                            visible: false;
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 坐标
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("坐标");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_pointcontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.centerIn: parent;
                                                            anchors.verticalCenterOffset: 2;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_pointvalue);
                                                            visible: false;
                                                        }
                                                    }
                                                }
                                                Rectangle{ width:parent.width;height:1; color:"#00ceff"}
                                                /*-------------------------------------------
                                                * 速度
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("速度");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_sogcontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
//                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
//                                                        TextEdit {
//                                                            anchors.centerIn: parent;
//                                                            anchors.verticalCenterOffset: 2;
//                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
//                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
//                                                            smooth: true; text: qsTr(m_sogvalue);
//                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 位置
                                                ---------------------------------------------*/
                                                Row{
                                                    anchors.rightMargin: 10;
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("位置");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_locationcontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
//                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.centerIn: parent;
                                                            anchors.verticalCenterOffset: 2;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_locationvalue);
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 吃水
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("吃水");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_draughtcontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
//                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
//                                                        TextEdit {
//                                                            anchors.centerIn: parent;
//                                                            anchors.verticalCenterOffset: 2;
//                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
//                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
//                                                            smooth: true; text: qsTr(m_draughtvalue);
//                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 瞄重
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 22; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("瞄重");
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        TextEdit {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.left: parent.left; anchors.leftMargin: 5;
                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
                                                            verticalAlignment: TextEdit.AlignVCenter;
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr(m_anchorcontent);
                                                        }
                                                    }
                                                    Rectangle{
                                                        width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
//                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
//                                                        TextEdit {
//                                                            anchors.centerIn: parent;
//                                                            anchors.verticalCenterOffset: 2;
//                                                            selectByMouse: true; readOnly: true; color: "#ffffff";
//                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
//                                                            smooth: true; text: qsTr(m_anchorvalue);
//                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 保护详情
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 132; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("保护详情");
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * A相
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsAcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * B相
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsBcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * C相
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsCcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 保护方式
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsProtectcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 埋设深度
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsRealdepthcontent);
                                                            }
                                                        }

                                                        /*-------------------------------------------
                                                        * 水深
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsWaterDepthcontent);
                                                            }
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * A相值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsAvalue);
                                                                //visible: false;
                                                            }


                                                        }
                                                        /*-------------------------------------------
                                                        * B相值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsBvalue);
                                                                //visible: false;
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * C相值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsCvalue);
                                                                //visible: false;
                                                            }
                                                        }

                                                        /*-------------------------------------------
                                                        * 保护方式值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(/*m_scmsProtectvalue*/m_scmsProtectevaluate);
                                                                //visible: false;
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 埋设深度值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsRealDepthvalue);
                                                                //visible: false;
                                                            }
                                                        }

                                                        /*-------------------------------------------
                                                        * 水深值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_scmsWaterDepthvalue);
                                                                //visible: false;
                                                            }
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 海况
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 88; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("海况");
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 风速
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_windcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 潮流
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_treadcontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 浪高
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr("--");
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 潮汐
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_tidalcontent);
                                                            }
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 风速值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_windvalue);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 潮流值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_treadvalue);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 浪高值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr("--");
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 潮汐值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_tidalvalue);
                                                            }
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 电气量
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 44; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("电气量");
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 电压
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_voltagecontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 电流
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_electriccontent);
                                                            }
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 电压值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_voltagevalue);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 电流值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_electricvalue);
                                                            }
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 非电气量
                                                ---------------------------------------------*/
                                                Row{
                                                    Rectangle{
                                                        width: 61; height: 44; color: Qt.rgba(1,1,1,0.02);
                                                        border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                        Text{
                                                            anchors.verticalCenterOffset: 2;
                                                            anchors.centerIn: parent; color: "#ffffff";
                                                            font{ family: localFont.name; pointSize: 9; bold: false; }
                                                            smooth: true; text: qsTr("非电气量");
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 油压
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_hydrauliccontent);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 油流
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 203; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                anchors.verticalCenterOffset: 2;
                                                                anchors.left: parent.left; anchors.leftMargin: 5;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                verticalAlignment: TextEdit.AlignVCenter;
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_oilFlowcontent);
                                                            }
                                                        }
                                                    }
                                                    Column{
                                                        /*-------------------------------------------
                                                        * 油压值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_hydraulicvalue);
                                                            }
                                                        }
                                                        /*-------------------------------------------
                                                        * 油流值
                                                        ---------------------------------------------*/
                                                        Rectangle{
                                                            width: 45; height: 22; color: Qt.rgba(1,1,1,0.02); clip: true;
                                                            border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                            TextEdit {
                                                                anchors.centerIn: parent;
                                                                anchors.verticalCenterOffset: 2;
                                                                selectByMouse: true; readOnly: true; color: "#ffffff";
                                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                                smooth: true; text: qsTr(m_oilFlowvalue);
                                                            }
                                                        }
                                                    }
                                                }
                                                Row{
                                                    width: parent.width; height: 32; spacing: 6;
                                                    Text {
                                                        anchors.verticalCenter: parent.verticalCenter; color: "#ffffff";
                                                        font{ family: localFont.name; pointSize: 9; bold: false; }
                                                        smooth: true; text: qsTr(" 预警总值:");
                                                    }
                                                    TextEdit {
                                                        width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                        selectByMouse: true; readOnly: true;
                                                        font{ family: localFont.name; pointSize: 9; bold: false; }
                                                        smooth: true; text: m_totalValues;
                                                    }
                                                }

                                            }
                                        }
                                        Item{ width: parent.width; height: 18; }
                                        Connections{target:alarmDetail;
                                            onSendMarkText:{
                                                remarkText.text = markText;
                                            }
                                        }

                                        /*-------------------------------------------
                                        * 备注
                                        ---------------------------------------------*/
                                        Row {
                                            width: parent.width; height: 26;
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter;
                                                anchors.verticalCenterOffset: -2;
                                                width: 45; color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 9; bold: false; }
                                                smooth: true; text: qsTr(" 备  注:");
                                            }
                                            Rectangle {
                                                width: parent.width-80; height: 24; color: "#cccccc";
                                                TextInput {
                                                    id: remarkText;
                                                    anchors.fill: parent; anchors.margins: 4;
                                                    font{ family: localFont.name; pointSize: 9; bold: false; italic: false; }
                                                    smooth: true; focus: true; cursorVisible: true; selectByMouse: true; color: "#1a1a1a";
                                                    maximumLength: 40; text: m_remarks;
                                                    readOnly: true;
                                                }
                                            }
                                            PushButtons {
                                                width: 30;  height: 23;
                                                label: qsTr("...")
                                                onClicked: { alarmDetail.markEdit(); }
                                            }
                                        }
                                        Item{ width: parent.width; height: 5; }
                                        Item {
                                            width: parent.width; height: 30;
                                            PushButtons {
                                                width: 90;  height: 23;
                                                anchors.right: parent.right; anchors.rightMargin: 3;
                                                anchors.verticalCenter: parent.verticalCenter;
                                                label: qsTr("解除告警")
                                                onClicked: { alarmDetail.relieveAlarmEvent(m_idx,remarkText.text); clearAlarm(); }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Connections{ target:alarmDetail ; onSendAlarmDetail: {
            m_idx = idx;                                //编号
            m_alarmtime = alarmtime;                    //告警时间(格式:yyyy-MM-dd HH:mm:ss)
            m_shipnamecontent = shipnamecontent;        //船名预警内容
            m_shipnameevaluate = shipnameevaluate;      //船名评估
            m_shipnamevalue = shipnamevalue;            //船名取值
            m_mmsicontent = mmsicontent;                //MMSI
            m_mmsievaluate = mmsievaluate;              //MMSI
            m_mmsivalue = mmsivalue;                    //MMSI
            m_pointcontent = pointcontent;              //坐标
            m_pointevaluate = pointevaluate;            //坐标
            m_pointvalue = pointvalue;                  //坐标
            m_sogcontent = sogcontent;                  //速度
            m_sogevaluate = sogevaluate;                //速度
            m_sogvalue = sogvalue;                      //速度
            m_locationcontent = locationcontent;        //位置
            m_locationevaluate = locationevaluate;      //位置
            m_locationvalue = locationvalue;            //位置
            m_draughtcontent = draughtcontent;          //吃水
            m_draughtevaluate = draughtevaluate;        //吃水
            m_draughtvalue = draughtvalue;              //吃水
            m_anchorcontent = anchorcontent;            //瞄重
            m_anchorevaluate = anchorevaluate;          //瞄重
            m_anchorvalue = anchorvalue;                //瞄重
            m_scmsAcontent = scmsAcontent;              //A相
            m_scmsAevaluate = scmsAevaluate;            //A相
            m_scmsAvalue = scmsAvalue;                  //A相
            m_scmsBcontent = scmsBcontent;              //B相
            m_scmsBevaluate = scmsBevaluate;            //B相
            m_scmsBvalue = scmsBvalue;                  //B相
            m_scmsCcontent = scmsCcontent;              //C相
            m_scmsCevaluate = scmsCevaluate;            //C相
            m_scmsCvalue = scmsCvalue;                  //C相
            m_scmsProtectcontent = protectContent;      //保护方式
            m_scmsProtectevaluate = protectEvaluate;          //保护方式
            m_scmsProtectvalue = protectValue;           //保护方式
            m_scmsRealdepthcontent = buriedContent;         //掩埋深度
            m_scmsRealDepthevaluate = buriedEvaluate;          //掩埋深度
            m_scmsRealDepthvalue = buriedValue;           //掩埋深度
            m_scmsWaterDepthcontent = waterDepthContent;         //水深
            m_scmsWaterDepthevaluate = waterDepthEvaluate;          //水深
            m_scmsWaterDepthvalue = waterDepthValue;           //水深
            m_windcontent = windcontent;                //风速
            m_windevaluate = windevaluate;              //风速
            m_windvalue = windvalue;                    //风速
            m_treadcontent = treadcontent;              //潮流
            m_treadevaluate = treadevaluate;            //潮流
            m_treadvalue = treadvalue;                  //潮流
            m_tidalcontent = tidalcontent;              //潮汐
            m_tidalevaluate = tidalevaluate;            //潮汐
            m_tidalvalue = tidalvalue;                  //潮汐
            m_voltagecontent = voltagecontent;          //电压
            m_voltageevaluate = voltageevaluate;        //电压
            m_voltagevalue = voltagevalue;              //电压
            m_electriccontent = electriccontent;        //电流
            m_electricevaluate = electricevaluate;      //电流
            m_electricvalue = electricvalue;            //电流
            m_hydrauliccontent = hydrauliccontent;      //油压
            m_hydraulicevaluate = hydraulicevaluate;    //油压
            m_hydraulicvalue = hydraulicvalue;          //油压
            m_oilFlowcontent = oilFlowcontent;          //油流
            m_oilFlowevaluate = oilFlowevaluate;        //油流
            m_oilFlowvalue = oilFlowvalue;              //油流
            m_totalValues = totalValues;                //总值
            m_remarks = remarks;                        //备注
            m_warningtype =  warningtype;               //预警类型
            m_warninggrade = warninggrade;              //预警级别
        } }

    function clearAlarm()
    {
        m_idx = "---";                              //编号
        m_alarmtime = "---";                        //告警时间(格式:yyyy-MM-dd HH:mm:ss)
        m_shipnamecontent = "";                     //船名预警内容
        m_shipnameevaluate = "";                    //船名评估
        m_shipnamevalue = "";                       //船名取值
        m_mmsicontent = "";                         //MMSI
        m_mmsievaluate = "";                        //MMSI
        m_mmsivalue = "";                           //MMSI
        m_pointcontent = "";                        //坐标
        m_pointevaluate = "";                       //坐标
        m_pointvalue = "";                          //坐标
        m_sogcontent = "";                          //速度
        m_sogevaluate = "";                         //速度
        m_sogvalue = "";                            //速度
        m_locationcontent = "";                     //位置
        m_locationevaluate = "";                    //位置
        m_locationvalue = "";                       //位置
        m_draughtcontent = "";                      //吃水
        m_draughtevaluate = "";                     //吃水
        m_draughtvalue = "";                        //吃水
        m_anchorcontent = "";                       //瞄重
        m_anchorevaluate = "";                      //瞄重
        m_anchorvalue = "";                         //瞄重
        m_scmsAcontent = "";                        //A相
        m_scmsAevaluate = "";                       //A相
        m_scmsAvalue = "";                          //A相
        m_scmsBcontent = "";                        //B相
        m_scmsBevaluate = "";                       //B相
        m_scmsBvalue = "";                          //B相
        m_scmsCcontent = "";                        //C相
        m_scmsCevaluate = "";                       //C相
        m_scmsCvalue = "";                          //C相
        m_scmsProtectcontent = "";                  //保护方式
        m_scmsProtectevaluate = "";                  //保护方式
        m_scmsProtectvalue = "";                  //保护方式
        m_scmsRealdepthcontent = "";                  //埋设深度
        m_scmsRealDepthevaluate = "";                  //埋设深度
        m_scmsRealDepthvalue = "";                  //埋设深度
        m_scmsWaterDepthcontent = "";                  //水深
        m_scmsWaterDepthevaluate = "";                  //水深
        m_scmsWaterDepthvalue = "";                  //水深
        m_windcontent = "";                         //风速
        m_windevaluate = "";                        //风速
        m_windvalue = "";                           //风速
        m_treadcontent = "";                        //潮流
        m_treadevaluate = "";                       //潮流
        m_treadvalue = "";                          //潮流
        m_tidalcontent = "";                        //潮汐
        m_tidalevaluate = "";                       //潮汐
        m_tidalvalue = "";                          //潮汐
        m_voltagecontent = "";                      //电压
        m_voltageevaluate = "";                     //电压
        m_voltagevalue = "";                        //电压
        m_electriccontent = "";                     //电流
        m_electricevaluate = "";                    //电流
        m_electricvalue = "";                       //电流
        m_hydrauliccontent = "";                    //油压
        m_hydraulicevaluate = "";                   //油压
        m_hydraulicvalue = "";                      //油压
        m_oilFlowcontent = "";                      //油流
        m_oilFlowevaluate = "";                     //油流
        m_oilFlowvalue = "";                        //油流
        m_totalValues = "";                         //总值
        remarkText.text = "";                       //备注
        m_warningtype =  "---";                     //预警类型
        m_warninggrade = "---";                     //预警级别
    }
}
