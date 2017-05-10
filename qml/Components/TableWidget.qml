import QtQuick 1.1

Rectangle {
    id: tableWidget;
    width: 100; height: 62;
    property variant tableTitle: ["全部", "船名"];
    property variant alarmColor: ["#ffffff", "#ff0000","#FFC000","#ffff00","#4f81bd","#00ff00", "green"];
    property int columnWidth: 145;
    property int rowHeight: 31;
    property int scrollWidth: 6630;
    property int scrollHeight: parent.height;
    property alias currentIndex: tableView.currentIndex;
    property alias count: tableView.count;
    property alias delegate: tableView.delegate;
    property alias model: tableView.model;
    signal sendOrder( int indexx, bool isSort );
    property bool typereorder: true;
    property bool timereorder: true;
    property bool isorderimg: false;
    property bool isDisplayVBar: false;
    Flickable {
        id: tableFlickView; anchors.fill: parent; contentWidth: scrollWidth;
        Column{
            anchors.fill: parent; spacing: 0; anchors.margins: 1;
            Rectangle{
                id: titleRect; width: (parent.width > tableWidget.width ) ? parent.width : tableWidget.width; height: 31; color: "#e5e5e5";
                ListView {
                    id: tabaleTitleView;
                    anchors.fill: parent;
                    model: tableTitle;
                    delegate: titleDelegate;
                    orientation: ListView.Horizontal;
                    interactive: false; clip: true;
                    /*-------------------------------------------
                     * 代理
                     ---------------------------------------------*/
                    Component {
                        id: titleDelegate
                        Rectangle{
                            width: columnWidth; height: 31; color: "transparent"
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#e5e5e5" }
                                GradientStop { position: 0.5; color: "#e5e5e5" }
                                GradientStop { position: 1.0; color: "#e5e5e5" }
                            }
                            Text {
                                anchors.centerIn: parent; text: tableTitle[index]; color: "black";
                                font{ family: localFont.name; pointSize: 10; bold: true; }
                            }
                            BorderImage {
                                id: table_arrow;
                                width: 8; height: 6;
                                anchors.top: parent.top;
                                anchors.topMargin: 3;
                                anchors.horizontalCenter: parent.horizontalCenter;
                                horizontalTileMode: BorderImage.Stretch;
                                verticalTileMode: BorderImage.Stretch;
                                source: "qrc:/ecdisviewer/qml/images/table_arrow.png";
                                visible: ((index == 2 || index == 11)&&isorderimg) ? true : false;
                                smooth: true;
                            }
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    if( index == 2)
                                    {
                                        typereorder ? typereorder = false : typereorder = true;
                                        tableWidget.sendOrder( index,typereorder );
                                        typereorder ? table_arrow.rotation = 0 :  table_arrow.rotation = 180 ;
                                    }else if(index == 11)
                                    {
                                        timereorder ? timereorder = false : timereorder = true;
                                        timereorder ? table_arrow.rotation = 0 :  table_arrow.rotation = 180 ;
                                        tableWidget.sendOrder( index,timereorder );
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /*-------------------------------------------
            * GridView
            ---------------------------------------------*/
            ListView {
                id: tableView;
                width: parent.width; height: parent.height-titleRect.height;
                model: tableModel;
                delegate: tableDelegate;
                highlightFollowsCurrentItem: false;
                focus: true; /*clip: true;*/
                boundsBehavior: Flickable.StopAtBounds;
                highlightMoveDuration: 250;
                ScrollBar{
                    id: verticalScrollBar;
                    width: 12; height: tableView.height-6;
                    anchors.right: tableView.right;
                    orientation: Qt.Vertical;
                    position: tableView.visibleArea.yPosition ;
                    pageSize: tableView.visibleArea.heightRatio ;
                    opacity: 0;
                    onScaleYChanged:  {
                        var totalheight = model.count * tableView.currentItem.height + (model.count - 1) * tableView.spacing;
                        var viewhieght = tableView.height;
                        var subheight = totalheight - viewhieght;
                        var movedis = subheight * pos;
                        var index = Math.ceil(movedis / tableView.currentItem.height);
                        console.log("display index:"+index);
                        tableView.positionViewAtIndex(index, ListView.Beginning)
                    }
                }
            }

        }
    }
    /*-------------------------------------------
     * 船舶档案滚动条
     ---------------------------------------------*/
    states:[
        State {
            name: "ShowHBars"
            when: tableFlickView.movingHorizontally;
            PropertyChanges { target: horizontalScrollBar; opacity: 1; }
        },
        State {
            name: "ShowVBars"
            when: tableView.movingVertically || (model.count * tableView.currentItem.height) + (model.count - 1) * tableView.spacing > tableView.height
            PropertyChanges {
                target: verticalScrollBar; opacity: 1;
            }
        }
    ]

    transitions: Transition { NumberAnimation { properties: "opacity"; duration: 400 } }
    ScrollBar {
        id: horizontalScrollBar;
        width: tableFlickView.width-6; height: 12;
        anchors.bottom: tableFlickView.bottom;
        orientation: Qt.Horizontal;
        position: tableFlickView.visibleArea.xPosition;
        pageSize: tableFlickView.visibleArea.widthRatio;
        opacity: 0;
    }


}
