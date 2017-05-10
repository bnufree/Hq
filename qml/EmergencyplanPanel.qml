import QtQuick 1.1

Rectangle {
    width: 600; height: 400; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }

    property string m_idx: "";                        //主键
    property string m_code: "";                       //编号 长度 15
    property string m_createtime: "";                 //创建时间 yyyy年MM月dd日HH时mm分
    property string m_planname: "";                   //预案名称 长度 30
    property string m_warningtype: "";                //预警类型 长度 20
    property string m_warningarea: "";                //预警区域 长度 15
    property string m_warninggrade: "";               //预警级别 长度 20
    property string m_manager: "";                    //管理者     长度 15
    property string m_remark: "";                     //备注        长度 50
    property string m_title: "";                      //标题        长度 30
    property string m_details: "";                    //明细      长度 50
    property string m_step: "";                       //步骤           长度 200

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
                    text: qsTr("预案详单");
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
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "预案信息"; } }
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
                                    anchors.fill: parent;
                                    contentHeight: ((m_plannameText.height+m_detailsText.height+m_stepText.height)<580) ? 580 : (m_plannameText.height+m_detailsText.height+m_stepText.height+80);
                                    clip: true;
                                    Column {
                                        spacing: 10; width: parent.width; anchors.left: parent.left; anchors.leftMargin: 6; anchors.right: parent.right; anchors.rightMargin: 6;
                                        Row{
                                            spacing: 6;
                                            Text {
                                                anchors.leftMargin: 4;
                                                color: "#f1f1f1"; smooth: true; text: qsTr("预案名称:");
                                                font{ family: localFont.name; pointSize: 10; bold: true; }
                                            }
                                            TextEdit {
                                                id: m_plannameText;
                                                color: "#f1f1f1"; smooth: true; text: m_planname; selectByMouse: true;
                                                KeyNavigation.tab: m_detailsText;
                                                readOnly: true;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                        Column {
                                            spacing: 6; width: parent.width;
                                            Text {
                                                anchors.leftMargin: 4;
                                                color: "#f1f1f1"; smooth: true; text: qsTr("预案明细:");
                                                font{ family: localFont.name; pointSize: 10; bold: true; }
                                            }
                                            TextEdit {
                                                id: m_detailsText;
                                                color: "#f1f1f1"; smooth: true; text: m_details; selectByMouse: true;
                                                KeyNavigation.tab: m_stepText;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                onFocusChanged: if(focus) selectAll(); readOnly: true;
                                                wrapMode: TextEdit.WordWrap; width: parent.width;
                                            }
                                        }
                                        Column {
                                            spacing: 6; width: parent.width;
                                            Text {
                                                anchors.leftMargin: 4;
                                                color: "#f1f1f1"; smooth: true; text: qsTr("预案步骤:");
                                                font{ family: localFont.name; pointSize: 10; bold: true; }
                                            }
                                            TextEdit {
                                                id: m_stepText;
                                                color: "#f1f1f1"; smooth: true; text: m_step; selectByMouse: true;
                                                KeyNavigation.tab: m_plannameText;
                                                font{ family: localFont.name; pointSize: 11; bold: false; }
                                                onFocusChanged: if(focus) selectAll(); readOnly: true;
                                                wrapMode: TextEdit.WordWrap; width: parent.width;
                                            }
                                        }
                                    }
                                }
                            }


                            Connections{ target:EmergencyplanModel;
                                onSendEmergencyplanByEventId: {
                                    m_idx = idx;                                //主键
                                    m_code = code;                              //编号 长度 15
                                    m_createtime = createtime;                  //创建时间 yyyy年MM月dd日HH时mm分
                                    m_planname = planname;                      //预案名称 长度 30
                                    m_warningtype = warningtype;                //预警类型 长度 20
                                    m_warningarea = warningarea;                //预警区域 长度 15
                                    m_warninggrade = warninggrade;              //预警级别 长度 20
                                    m_manager = manager;                        //管理者     长度 15
                                    m_remark = remark;                          //备注        长度 50
                                    m_title = title;                            //标题        长度 30
                                    m_details = details;                        //明细      长度 50
                                    m_step = step;                              //步骤           长度 200
                                } }
                        }
                    }
                }
            }
        }
    }
}
