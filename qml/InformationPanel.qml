import QtQuick 1.1

Rectangle {
    width: 600; height: 400; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property string m_shipName: "";
    property string m_callSign: "---";
    property string m_mmsi: "";
    property string m_imo: "---";
    property string m_shipType: "---";
    property string m_shipLength: "---";
    property string m_shipWidth: "---";
    property string m_draught: "---";
    property string m_anchorWeight: "---";

    property string m_naviStatus: "---";
    property string m_heading: "---";
    property string m_cog: "---";
    property string m_rot: "---";
    property string m_sog: "---";
    property string m_dest: "---";
    property string m_eta: "---";
    property string m_timeStamp: "---";
    property string m_airTimesOver: "---";
    property string m_shipsMarked: "---";
    property string m_lon:"---";
    property string m_lat:"---";
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
                    text: qsTr("AIS数据");
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
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "船舶信息"; } }
                }
                /*-------------------------------------------
                 * 船舶档案
                ---------------------------------------------*/
                Rectangle {
                    id:shipFileRectangle
                    anchors.bottom: parent.bottom;
                    width: 72; height: 20; clip: true; color: shipFileMouse.containsMouse && m_mmsi ? "#508cc0" : "#f79d00";
                    radius: 1;
                    Text {
                        anchors.centerIn: parent; color: shipFileMouse.containsMouse && m_mmsi ? "#FFFFFF": "#010101";
                        smooth: true; text: qsTr("船舶档案");
                        font{ family: localFont.name; pointSize: 9; bold: true; }
                    }
                    MouseArea { id: shipFileMouse; anchors.fill: parent; hoverEnabled: false; onClicked: { inforPanel.dealOpenShipsFile( m_mmsi,m_shipName ); } }
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
                    * 船舶信息
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
                                    smooth: true; text: qsTr("静态信息");
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
                                width: parent.width; height: 248;
                                Flickable {
                                    anchors.fill: parent; contentHeight: 250; clip: true;
                                    Column{
                                        anchors.fill: parent; anchors.leftMargin: 10; anchors.topMargin: 8;
                                        /*-------------------------------------------
                                        * 船名
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("船  名:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_shipName;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 呼号
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("呼  号:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_callSign;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * MMSI
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("MMSI:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_mmsi;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * IMO
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("IMO:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_imo;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 船型
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("船类型:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_shipType;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 船长
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("船 长:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_shipLength;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 船宽
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("船 宽:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_shipWidth;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 吃水
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("吃 水:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_draught;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 锚重
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 52; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("锚 重:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_anchorWeight;
                                            }
                                        }
                                    }
                                }
                            }

                            /*-------------------------------------------
                            * 动态信息
                            ---------------------------------------------*/
                            Item { width: parent.width; height: 5;}
                            Item {
                                width: parent.width; height: 22;
                                Text {
                                    anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 0; } color: "#00ceff";
                                    font{ family: localFont.name; pointSize: 11; bold: true; }
                                    smooth: true; text: qsTr("动态属性");
                                }
                            }
                            /*-------------------------------------------
                            * 线
                            ---------------------------------------------*/
                            Row{
                                width: parent.width; height: 2; spacing: -1;
                                Rectangle{ id: bold1Line; width: 65; height: 3; color: "#00ceff"; radius: 2; }
                                Rectangle{ width: parent.width-bold1Line.width-expansion1btn.width; height: 1; color: "#00ceff"; anchors.top: parent.bottom; }
                                BorderImage {
                                    id: expansion1btn; width: 31; height: 6;
                                    anchors.top: parent.bottom;
                                    horizontalTileMode: BorderImage.Stretch;
                                    verticalTileMode: BorderImage.Stretch;
                                    source: "qrc:/ecdisviewer/qml/images/expansion.png";
                                    smooth: true;
                                    MouseArea { anchors.fill: parent; }
                                }
                            }


                            /*-------------------------------------------
                            * 动态信息
                            ---------------------------------------------*/
                            Item{
                                width: parent.width; height: 273; clip: true;
                                Flickable {
                                    anchors.fill: parent; contentHeight: 280;
                                    Column{
                                        anchors.fill: parent; anchors.leftMargin: 10; anchors.topMargin: 8;
                                        /*-------------------------------------------
                                        * 航行状态
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("航行状态:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_naviStatus;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 经度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("经度:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_lon;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 纬度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("纬度:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_lat;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 航首向
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("航首向:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_heading;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 航迹向
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("航迹向:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_cog;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 转向率
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("转向率:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_rot;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 航速
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("航速:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_sog;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 目的地
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("目的地:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_dest;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 预到时间
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("预到时间:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_eta;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 最后时间
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("最后时间:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_timeStamp;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 过航次数
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("过航次数:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_airTimesOver;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 船舶标注
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 72; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("船舶标注:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_shipsMarked;
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
    Connections {
        target: inforPanel;
        onSendAttentionToPanel: {
            m_shipName = shipName;
            m_callSign = callSign;
            m_mmsi = mmsi;
            m_imo = imo;
            m_shipType = shipType;
            m_shipLength = shipLength;
            m_shipWidth = shipWidth;
            m_draught = draught;
            m_anchorWeight = anchorWeight;

            m_naviStatus = naviStatus;
            m_heading = heading;
            m_cog = cog;
            m_rot = rot;
            m_sog = sog;
            m_dest = dest;
            m_eta = eta;
            m_timeStamp = timeStamp;
            m_airTimesOver = airTimesOver;
            m_shipsMarked = shipsMarked;
            m_lon = lon;
            m_lat = lat;
        }
    }
    Connections{
            target: inforPanel;
            onSendRoleState:{
                shipFileRectangle.enabled = roleState;
            }
        }
}
