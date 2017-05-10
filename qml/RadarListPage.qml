import QtQuick 1.1

Rectangle {
    width: 600; height: 900; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }
    property int columnWidth: 100;
    property int widgetColumnWidth: 89;
    property int rowHeight: 31;
    property int m_target_Number_I161: 0;
    property string m_target_140: "0";
    property double m_target_042x: 0.0;
    property double m_target_042y: 0.0;
    property double m_target_speed: 0.0;
    property double m_target_sngle: 0.0;
    property string m_target_170_CNF: "0";
    property string m_target_170_cst: "0";
    property string m_target_lon: "0";
    property string m_target_lat: "0";


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
                    text: qsTr("雷达数据");
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
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "雷达信息"; } }
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
                    * 雷达信息
                    ---------------------------------------------*/
                    Item{
                        width: centersview.width; height: centersview.height;
                        Column{
                            anchors.fill: parent; spacing: 0;
                            /*-------------------------------------------
                            * 静态信息
                            ---------------------------------------------*/
                            Item { width: parent.width; height: 5;}
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
                            * 静态信息
                            ---------------------------------------------*/
                            Item{
                                width: parent.width; height: 748;
                                Flickable {
                                    anchors.fill: parent; contentHeight: 250; clip: true;
                                    Column{
                                        anchors.fill: parent; anchors.leftMargin: 10; anchors.topMargin: 8;
                                        /*-------------------------------------------
                                        * 数据源唯一标识符
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("航迹号:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_Number_I161;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 时间
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("时间:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_140;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 极坐标测量位置
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("笛卡尔坐标-X轴:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_042x;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * WGS84坐标计算位置
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("笛卡尔坐标-Y轴:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_042y;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 经度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
                                                text: qsTr("经度:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text:  m_target_lon;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 纬度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
                                                text: qsTr("纬度:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_lat;
                                            }
                                        }
                                        /*-------------------------------------------
                                        *
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
//                                                text: qsTr("数据源唯一标识符:  ");
                                                text: qsTr("径向速度:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_speed;
                                            }
                                        }
                                        /*-------------------------------------------
                                        *
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
//                                                text: qsTr("目标报告描述符:  ");
                                                text: qsTr("角度:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_sngle;
                                            }
                                        }
                                        /*-------------------------------------------
                                        *
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
//                                                text: qsTr("振荡波强度检测:  ");
                                                text: qsTr("航迹状态:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_170_CNF;
                                            }
                                        }
                                        /*-------------------------------------------
                                        *
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 150; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true;
//                                                text: qsTr("迪卡尔坐标航迹计算速度-X:  ");
                                                text: qsTr("推算:  ");
                                            }
                                            TextEdit {
                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_target_170_cst;
                                            }
                                        }

//                                        Row{
//                                            width: parent.width; height: 26;
//                                            Text {
//                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
//                                                font{ family: localFont.name; pointSize: 11; bold: false; }
//                                                smooth: true; text: qsTr("迪卡尔坐标航迹计算速度-Y:  ");
//                                            }
//                                            TextEdit {
//                                                width: 200; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
//                                                selectByMouse: true; readOnly: true;
//                                                font{ family: localFont.name; pointSize: 11; bold: false; }
//                                                smooth: true; text: m_target_CartesianTrkVel_y;
//                                            }
//                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Connections{ target:RadarListPageModel ;
        onSendRadarList:{
             m_target_Number_I161 = I161_TrkNb;
             m_target_140 =  ToD;
             m_target_042x = cartesianCoordinate_x;
             m_target_042y = cartesianCoordinate_y;
             m_target_speed = targetCartesianTrkVel_x;
             m_target_sngle = targetCartesianTrkVel_y;
             m_target_170_CNF = I170_CNF;
             m_target_170_cst = I170_CST;
             m_target_lon = targetPosInCartesian_I042_x;
             m_target_lat = targetPosInCartesian_I042_y;
        }
    }

}
