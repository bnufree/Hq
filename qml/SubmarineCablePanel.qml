import QtQuick 1.1

Rectangle {
    width: 600; height: 400; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }

    property string m_idx: "---";                       //序号
    property string m_code: "---";                      //编号
    property string m_lat: "---";                       //纬度
    property string m_lon: "---";                       //经度
    property string m_kp: "---";                        //KP值
    property string m_realdepth: "---";                 //实际埋深
    property string m_targetdepth: "---";               //目标埋深
    property string m_depthgap: "---";                  //埋深差值
    property string m_anchor: "---";                    //抗击锚重
    property string m_rockprotect: "---";               //抛石保护 长度 20
    property string m_remark: "---";                    //备注   长度 20

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
                    text: qsTr("海缆路由");
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
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "海缆数据"; } }
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
                            Item { width: parent.width; height: 10;}
                            /*-------------------------------------------
                            * 详细信息
                            ---------------------------------------------*/
                            Item{
                                width: parent.width; height: parent.height-30;
                                Flickable {
                                    anchors.fill: parent; contentHeight: 600; clip: true;

                                    Column{
                                        anchors.fill: parent; anchors.leftMargin: 10; anchors.topMargin: 8; spacing: 6;
                                        /*-------------------------------------------
                                        * 编号
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("编号:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_code;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 纬度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
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
                                        * 经度
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
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
                                        * KP值
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("KP值:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_kp;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 实际埋深
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("实际埋深:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_realdepth;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 目标埋深
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("目标埋深:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_targetdepth;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 埋深差值
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("埋深差值:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_depthgap;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 抗击锚重
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("抗击锚重:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_anchor;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 抛石保护
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("抛石保护:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_rockprotect;
                                            }
                                        }
                                        /*-------------------------------------------
                                        * 备注
                                        ---------------------------------------------*/
                                        Row{
                                            width: parent.width; height: 26;
                                            Text {
                                                width: 73; anchors.verticalCenter: parent.verticalCenter;  color: "#ffffff";
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: qsTr("备注:");
                                            }
                                            TextEdit {
                                                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#00ff03";
                                                selectByMouse: true; readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                smooth: true; text: m_remark;
                                            }
                                        }
                                    }
                                    Connections {
                                        target: SubmarineCablePanels;
                                        onSendCableData: {
                                            m_idx = idx;                            //序号
                                            m_code = code;                          //编号
                                            m_lat = lat;                            //纬度
                                            m_lon = lon;                            //经度
                                            m_kp = kp;                              //KP值
                                            m_realdepth = realdepth;                //实际埋深
                                            m_targetdepth = targetdepth;            //目标埋深
                                            m_depthgap = depthgap;                  //埋深差值
                                            m_anchor = anchor;                      //抗击锚重
                                            m_rockprotect = rockprotect;            //抛石保护 长度 20
                                            m_remark = remark;                      //备注   长度 20
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
