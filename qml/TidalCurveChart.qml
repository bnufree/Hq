import QtQuick 1.1
import "Components"

Rectangle{
    id: roots;
    width: 1600; height: 42;
    color: Qt.rgba(0.06,0.31,0.55,0.4);
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }
    property double maxtidal: 0;
    property double mintidal: 0;
    Column{
        anchors.fill: parent;
        Rectangle{ width: parent.width; height: 1; color: "#818181"; }
        Rectangle{
            width: parent.width; height: 39; color: Qt.rgba(0.06,0.31,0.55,0.9); z: 1;
            Row{
                anchors.fill: parent;
                Item{ width: 6; height: parent.height; }
                TextEdit {
                    anchors.verticalCenter: parent.verticalCenter;  color: "#f1f1f1";
                    selectByMouse: true; readOnly: true;
                    font{ family: localFont.name; pointSize: 11; bold: true; }
                    smooth: true; text: "时区：-0800(东8区) 潮高基准面：在平均海平面下150cm";
                }
                Item{ width: 10; height: parent.height; }
                TextEdit {
                    anchors.verticalCenter: parent.verticalCenter;  color: "#f1f1f1";
                    selectByMouse: true; readOnly: true;
                    font{ family: localFont.name; pointSize: 11; bold: true; }
                    smooth: true; text: "最高潮高："+maxtidal;
                }
                Item{ width: 10; height: parent.height; }
                TextEdit {
                    anchors.verticalCenter: parent.verticalCenter;  color: "#f1f1f1";
                    selectByMouse: true; readOnly: true;
                    font{ family: localFont.name; pointSize: 11; bold: true; }
                    smooth: true; text: "最低潮高："+ mintidal;
                }
                Item{
                    width: 60; height: parent.height;
                    Text {
                        anchors.centerIn: parent;
                        text: qsTr("起始时间:"); color: "#FFFFFF";
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                    }
                    visible: false;
                }
                LineEdit{
                    anchors.verticalCenter: parent.verticalCenter;
                    height: 23; radius: 2;
                    visible: false;
                }
                Item{ width: 10; height: parent.height; }
                Item{
                    width: 60; height: parent.height;
                    Text {
                        anchors.centerIn: parent;
                        text: qsTr("结束时间:");color: "#FFFFFF";
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                    }
                    visible: false;
                }
                LineEdit{
                    anchors.verticalCenter: parent.verticalCenter;
                    height: 23; radius: 2;
                    visible: false;
                }
                Item{ width: 10; height: parent.height; }

                PushButtons{
                    anchors.verticalCenter: parent.verticalCenter;
                    width: 64; height: 23;
                    label: qsTr("刷新");
                    visible: false;
                }
            }
            Item {
                width: 36; height: parent.height;
                anchors.right: parent.right; //anchors.rightMargin: 2;
                anchors.verticalCenter: parent.verticalCenter;
                BorderImage {
                    width: closeMouseArea.containsMouse ? 16: 14; height: closeMouseArea.containsMouse ? 16: 14; smooth: true;
                    anchors.centerIn: parent;
                    anchors.verticalCenterOffset: -2;
                    border { left: 1; top: 1; right: 1; bottom: 1 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/closebutton.png";
                    MouseArea{
                        id : closeMouseArea; anchors.fill: parent; hoverEnabled: true; onClicked: tidalCurveModel.sendInfoPanelsVisible(false);
                    }
                }
            }
        }
    }
    Connections{ target: tidalCurveModel; onSendMaxMinTidal: { maxtidal = maxx; mintidal = minx; } }
}
