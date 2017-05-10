import QtQuick 1.1

Rectangle {
    width: 600; height: 400; color: Qt.rgba(0.2,0.2,0.2,0.3);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
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
                    text: qsTr("位置标注");
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
                    ListModel { id: tabListsModel; ListElement { idx: "0"; names: "位置信息"; } }
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
//                                    anchors.fill: parent; contentHeight: 600; clip: true;
                                    anchors.fill: parent; contentHeight: parent.height; clip: true;
                                    ListView {
                                        id: listView
                                        anchors.fill: parent;
                                        anchors.margins: 2;
                                        model: locationModel;
                                        delegate: recipeDelegate;
                                        preferredHighlightBegin: 0;
                                        preferredHighlightEnd: 0
                                        spacing: 1;
                                        flickDeceleration: 2000;
                                        ListModel { id: locationModel; }
                                        Component {
                                            id: recipeDelegate
                                            Rectangle {
                                                id: recipe
                                                property real detailsOpacity : 0;
                                                width: listView.width; height: 38;
                                                color: Qt.rgba(1,1,1,0.0); clip: true;
                                                //border{ width: 1; color: Qt.rgba(1,1,1,0.1); }
                                                MouseArea { anchors.fill: parent; onClicked: (recipe.state == 'Details') ? recipe.state = '' : recipe.state = 'Details'; }
                                                /*-------------------------------------------
                                                * 列信息
                                                ---------------------------------------------*/
                                                Item {
                                                    id: listItem;
                                                    anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 10; }
                                                    height: parent.height; width: parent.width;
                                                    Row {
                                                        anchors.fill: parent;  spacing: 6;
                                                        BorderImage {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            width: 20; height: 20; smooth: true;
                                                            horizontalTileMode: BorderImage.Stretch;
                                                            verticalTileMode: BorderImage.Stretch;
                                                            source: "qrc:/ecdisviewer/qml/images/markeicon.png";
                                                        }
                                                        Text {
                                                            anchors.verticalCenter: parent.verticalCenter;
                                                            font{ family: localFont.name; pointSize: 10; bold: false; }
                                                            color: "#eeeeee"; text: names;
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 详细
                                                ---------------------------------------------*/
                                                Item {
                                                    id: details;
                                                    width: parent.width;
                                                    anchors.left: parent.left;
                                                    opacity: recipe.detailsOpacity;
                                                    Column{
                                                        anchors.fill: parent; anchors.margins: 10; spacing: 10;
                                                        Row {
                                                            spacing: 10;
                                                            BorderImage {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                width: 28; height: 28; smooth: true;
                                                                horizontalTileMode: BorderImage.Stretch;
                                                                verticalTileMode: BorderImage.Stretch;
                                                                source: "qrc:/ecdisviewer/qml/images/markeicon.png";
                                                            }
                                                            Text {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                color: "#ffffff"; text: names;
                                                            }
                                                        }
                                                        Row{
                                                            spacing: 20;
                                                            Row {
                                                                spacing: 6;
                                                                Text {
                                                                    anchors.verticalCenter: parent.verticalCenter;
                                                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                    color: "#ffffff"; text: "经度:";
                                                                }
                                                                Text {
                                                                    anchors.verticalCenter: parent.verticalCenter;
                                                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                    color: "#ffffff"; text: longitude;
                                                                }
                                                            }
                                                            Row {
                                                                spacing: 6;
                                                                Text {
                                                                    anchors.verticalCenter: parent.verticalCenter;
                                                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                    color: "#ffffff"; text: "纬度:"
                                                                }
                                                                Text {
                                                                    anchors.verticalCenter: parent.verticalCenter;
                                                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                    color: "#ffffff"; text: latitude/*longitude*/;
                                                                }
                                                            }
                                                        }
                                                        Row {
                                                            spacing: 10;
                                                            Text {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                color: "#ffffff"; text: "备注:"
                                                            }
                                                            Text {
                                                                anchors.verticalCenter: parent.verticalCenter;
                                                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                                                color: "#ffffff"; text: markes
                                                            }
                                                        }
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 关闭按钮
                                                ---------------------------------------------*/
                                                Rectangle {
                                                    anchors { right: recipe.right; rightMargin: 5; top: recipe.top; topMargin: 6; }
                                                    width: 12; height: 12;
                                                    opacity: recipe.detailsOpacity;
                                                    MouseArea{ anchors.fill: parent; onClicked: MarkePanelModel.sendDeleteMarke(indexs); }
                                                    BorderImage {
                                                        anchors.centerIn: parent;
                                                        horizontalTileMode: BorderImage.Stretch;
                                                        verticalTileMode: BorderImage.Stretch;
                                                        width: 12; height: 12; smooth: true;
                                                        source: "qrc:/ecdisviewer/qml/images/closebutton.png"
                                                    }
                                                }
                                                /*-------------------------------------------
                                                * 状态及动画
                                                ---------------------------------------------*/
                                                states: State {
                                                    name: "Details"
                                                    PropertyChanges { target: recipe; color: Qt.rgba(1,1,1,0.2); }
                                                    PropertyChanges { target: listItem; visible: false; }
                                                    PropertyChanges { target: recipe; detailsOpacity: 1; x: 0 }
                                                    PropertyChanges { target: recipe; height: 120 }
                                                    PropertyChanges { target: recipe.ListView.view; interactive: false; }
                                                    PropertyChanges { target: recipe.ListView.view; explicit: true; contentY: recipe.y }
                                                }
                                                transitions: Transition {
                                                    ParallelAnimation {
                                                        ColorAnimation { property: "color"; duration: 500 }
                                                        NumberAnimation { duration: 300; properties: "detailsOpacity,x,contentY,height,width" }
                                                    }
                                                }
                                            }
                                        }
                                        Connections{ target:MarkePanelModel ; onSendClearMarkes: locationModel.clear(); }
                                        Connections{ target:MarkePanelModel ; onSendAddMarkeInfo: { locationModel.append( {"indexs": idx, "names": namess,"latitude": latitudes, "longitude": longitudes,"markes": markess } );}  }
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
