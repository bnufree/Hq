import QtQuick 1.1
import "Components"

Rectangle {
    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int shipNum: 0;
    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column{
                anchors.fill: parent; anchors.margins: 0; clip: true;
                Rectangle{
                    width: parent.width-1; height: 28;
                    border{ width: 1; color: "#dedede" } anchors.margins: 4;
                    Row{
                        anchors.fill: parent; spacing: 6;
                        Text {
                            anchors.verticalCenter: parent.verticalCenter;
                            font{ family: localFont.name; pointSize: 10; }
                            color: "#1a1a1a"; text: qsTr("  区域船舶数:");
                        }
                        TextEdit {
                            anchors.verticalCenter: parent.verticalCenter;
                            width: 160; color: "#1a1a1a";
                            selectByMouse: true; readOnly: true;
                            font{ family: localFont.name; pointSize: 10; }
                            smooth: true; text: shipNum;
                        }
                        Connections{ target: FrameSelectModel; onSendShipNum: shipNum = number; }
                    }
                }
                ListView {
                    id: selectionTable;
                    width: parent.width; height: parent.height-28;
                    model: selectionModel; anchors.margins: 1;
                    ListModel { id: selectionModel; }
                    Connections{ target: FrameSelectModel; onClear:selectionModel.clear() }
                    Connections{ target: FrameSelectModel; onSendSelectionInfo: selectionModel.append( {"item0": mmsi, "item1":shipName} ); }
                    delegate: titleDelegate;
                    orientation: ListView.Vertical; clip: true;
                    /*-------------------------------------------
                     * 代理
                     ---------------------------------------------*/
                    Component {
                        id: titleDelegate;
                        Rectangle{
                            width: selectionTable.width; height: 23;
                            color:ListView.isCurrentItem ? "#8edcf3" : "#ffffff";
                            border{ width: 1; color: "#dedede" }
                            Row{
                                anchors.centerIn: parent;
                                Rectangle{ width: 1; height: 23; color: "#dedede" }
                                Item {
                                    width: 96; height: 23; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item0; color: "#202020" ;
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Rectangle{ width: 1; height: 23; color: "#dedede" }
                                Item {
                                    width: selectionTable.width-96; height: 23; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item1; color: "#202020" ;
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Rectangle{ width: 1; height: 23; color: "#dedede" }
                            }
                            /*-------------------------------------------
                             * 响应鼠标点击项动作
                             ---------------------------------------------*/
                            MouseArea {
                                anchors.fill: parent;
                                onClicked: { selectionTable.currentIndex = index; }
                                onDoubleClicked: FrameSelectModel.sendDoubleClick(item0);
                            }
                        }
                    }
                }
            }
        }
    }
}
