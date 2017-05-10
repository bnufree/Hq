import QtQuick 1.1
import "Components"
Item{
    id: roots;
    width: 1600; height: 42;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int columnWidth: 200;
    property int rowHeight: 31;
    Rectangle{
        anchors.fill: parent; color: Qt.rgba(0.06,0.31,0.55,0.8);
        Column{
            Rectangle{ width: parent.width; height: 1; color: "#818181"; }
            anchors.fill: parent;
            /*-------------------------------------------
            * 翻页操作
            ---------------------------------------------*/
            Row{
                id: titleRect;
                width: parent.width; height: 39; z:1;anchors.leftMargin: 5;clip: true
                Text {
                    width: 41; height: 26;
                    anchors.verticalCenter: parent.verticalCenter;
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr(" 首页 "); color: fristPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                    MouseArea{
                        id : fristPageMouseArea;
                        anchors.fill: parent; hoverEnabled: true;
                        onClicked: {
                            redarInforModel.setPageNo(1);
                            redarInforModel.getRadarList(redarInforModel.pageSize, redarInforModel.pageNo)
                        }
                    }
                }
                Item { width: 3; height: 26; }
                Text {
                    width: 46; height: 26;
                    anchors.verticalCenter: parent.verticalCenter;
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("上一页"); color: previousPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                    MouseArea{
                        id : previousPageMouseArea;
                        anchors.fill: parent; hoverEnabled: true;
                        onClicked: {
                            redarInforModel.setPageNo(redarInforModel.pageNo-1);
                            redarInforModel.getRadarList(redarInforModel.pageSize, redarInforModel.pageNo)
                        }
                    }
                }
                Item { width: 3; height: 26; }
                Text {
                    width: 46; height: 26;
                    anchors.verticalCenter: parent.verticalCenter;
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("下一页"); color: nextPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                    MouseArea{
                        id : nextPageMouseArea;
                        anchors.fill: parent; hoverEnabled: true;
                        onClicked: {
                            redarInforModel.setPageNo(redarInforModel.pageNo+1);
                            redarInforModel.getRadarList(redarInforModel.pageSize, redarInforModel.pageNo)
                        }
                    }
                }
                Item { width: 3; height: 26; }
                Text {
                    width: 41; height: 26;
                    anchors.verticalCenter: parent.verticalCenter;
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr(" 尾页 "); color: endPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                    MouseArea{
                        id : endPageMouseArea;
                        anchors.fill: parent; hoverEnabled: true;
                        onClicked: {
                            redarInforModel.setPageNo(redarInforModel.recordCount);
                            redarInforModel.getRadarList(redarInforModel.pageSize, redarInforModel.pageNo)
                        }
                    }
                }
                Item { width: 20; height: 26; }
                /*-------------------------------------------
                    * 当前页码
                    ---------------------------------------------*/
                LineEdit{
                    width: 50; height: 18; clip: true; color: "#010101"; anchors.verticalCenter: parent.verticalCenter;
                    text: redarInforModel.pageNo;
                    validator: IntValidator{ bottom: 0; top: redarInforModel.recordCount; }
                    Keys.onEnterPressed: redarInforModel.setPageNo(text);
                    Keys.onReturnPressed: redarInforModel.setPageNo(text);
                }
                Item { width: 1; height: 26; }
                /*-------------------------------------------
                    * 总页数
                    ---------------------------------------------*/
                Item {
                    width: 60; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                    Text {
                        anchors.fill: parent;
                        verticalAlignment: Text.AlignVCenter
                        color: "#CFCFCF"; text: " / "+ redarInforModel.recordCount + qsTr(" 页");
                        font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                    }
                }
                Item { width: 0; height: 26; }
                /*-------------------------------------------
                    * 总记录数
                    ---------------------------------------------*/
                Item {
                    width: 130; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                    Text {
                        anchors.fill: parent;
                        verticalAlignment: Text.AlignVCenter
                        color: "#CFCFCF"; text: "总数: "+ redarInforModel.rowCount + qsTr(" 条");
                        font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                    }
                }
            }
            Item {
                width: parent.width; height: parent.height-titleRect.height;
                /*-------------------------------------------
                 * 表格
                 ---------------------------------------------*/
                TableWidget{
                    id: tablesView;
                    anchors.fill: parent;
                    tableTitle: ["航迹号","时间","笛卡尔坐标X","笛卡尔坐标Y","经度","纬度","径向速度"/*"航迹计算速度X"*/,"角度"/*"航迹计算速度Y"*/,"航迹状态","推算"];
                    scrollWidth: 10*columnWidth;
                    delegate: topTableDelegate;
                    model: ListModel { id: topTableModel;}
                    /*-------------------------------------------
                     * 向模型添加数据
                     ---------------------------------------------*/
                    Connections {
                        target: redarInforModel;
                        //onSendClear:topTableModel.clear();
                        onSendRadarList: {
                            topTableModel.set(pos,  {
                                                    "item0": I161_TrkNb,
                                                    "item1": ToD,
                                                    "item2": cartesianCoordinate_x,
                                                    "item3": cartesianCoordinate_y,
                                                    "item4": targetPosInCartesian_I042_x,
                                                    "item5": targetPosInCartesian_I042_y,
                                                    "item6": targetCartesianTrkVel_x,
                                                    "item7": targetCartesianTrkVel_y,
                                                    "item8": I170_CNF,
                                                    "item9": I170_CST
                                                }
                                                )
                        }
                        }
                    /*-------------------------------------------
                    * 代理
                    ---------------------------------------------*/
                    Component {
                        id: topTableDelegate;
                        Rectangle{
                            width: tablesView.width-2; height: rowHeight;
                            color:ListView.isCurrentItem ? "#8edcf3" : "#FFFFFF";
                            border{ width: 1; color: "#dedede" }
                            Row{
                                anchors.fill: parent;
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item0; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item1; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item2; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item3; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item4; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item5; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item6; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item7; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item8; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item9; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 响应鼠标点击项动作
                             ---------------------------------------------*/
                            MouseArea {
                                anchors.fill: parent;
                                onClicked: { tableView.currentIndex = index;}
                                onDoubleClicked: redarInforModel.sendRadarIndex(item0);
                            }
                        }
                    }
                }
            }
        }
    }
}
