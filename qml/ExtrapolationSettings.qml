import QtQuick 1.1
import "Components"

Rectangle {
    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int rotationvalue: 0;
    property int rotation: 0;
    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column{
                anchors.fill: parent; anchors.margins: 10;
                Item{ width: parent.width; height: 10; }
                Row{
                    spacing: 3;
                    Item{ width: 6; height: parent.height; }
                    Text {
                        height: parent.height;
                        color: "#1a1a1a"; smooth: true; text: qsTr("外推周期");
                        font{ family: localFont.name; pointSize: 11; bold: false; }
                    }
                    LineEdit{
                        id: minuteEdit;
                        width: 60; height: 22; text: "3";
                        validator: IntValidator{ bottom: 0; top: 1000; }
                    }
                    Connections{ target: ExtrapolationModel; onSendSetExtrapolationValue: minuteEdit.text = values; }
                    Text {
                        height: parent.height;
                        color: "#1a1a1a"; smooth: true; text: qsTr("分钟");
                        font{ family: localFont.name; pointSize: 11; bold: false; }
                    }
                }
                Item{ width: parent.width; height: 15; }
                /*-------------------------------------------
                * 按钮
                ---------------------------------------------*/
                Rectangle{
                    width: parent.width; height: 35; color: "#ffffff";
                    Row{
                        anchors.right: parent.right; anchors.rightMargin: 12; spacing: 10;
                        anchors.verticalCenter: parent.verticalCenter;
                        PushButtons {
                            width: 80;  height: 23;
                            label: qsTr("+1分钟")
                            onClicked: {
                                minuteEdit.text = Math.abs(minuteEdit.text)+1;
                                ExtrapolationModel.sendExtrapolationValue(minuteEdit.text);
                            }
                        }
                        PushButtons {
                            width: 80;  height: 23;
                            label: qsTr("-1分钟")
                            onClicked: {
                                if(minuteEdit.text > 1)
                                {
                                    minuteEdit.text = Math.abs(minuteEdit.text)-1;
                                }
                                ExtrapolationModel.sendExtrapolationValue(minuteEdit.text);
                            }
                        }
                        PushButtons {
                            width: 80;  height: 23;
                            label: qsTr("恢复")
                            onClicked: {
                                minuteEdit.text = 3;
                                ExtrapolationModel.sendRecover();
                            }
                        }

                    }
                }
            }
        }
    }
}
