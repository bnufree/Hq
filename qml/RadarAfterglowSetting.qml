import QtQuick 1.1
import "Components"
Rectangle {
    width: 100
    height: 62
    color: "#ffffff";
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }
    Column{
        anchors{ fill: parent; margins: 4; } spacing: 2;
        Item{
            width: parent.width;height: 32;anchors.leftMargin: 10;
            Row{
                anchors.fill: parent
                Item{width: 20; height: parent.height;}
                ComboBox{
                    id:timeComboBox
                    anchors.verticalCenter: parent.verticalCenter;z:3
                    width: 128; height: 20;
                    ListModel {
                        id: comboBox1Model
                        ListElement { idx: "1"; name: "1"; }
                        ListElement { idx: "2"; name: "2"; }
                        ListElement { idx: "3"; name: "3"; }
                        ListElement { idx: "4"; name: "4"; }
                        ListElement { idx: "5"; name: "5"; }
                        ListElement { idx: "6"; name: "6"; }
                        ListElement { idx: "7"; name: "7"; }
                        ListElement { idx: "8"; name: "8"; }
                        ListElement { idx: "9"; name: "9"; }
                        ListElement { idx: "10"; name: "10"; }
                        ListElement { idx: "11"; name: "11"; }
                        ListElement { idx: "12"; name: "12"; }
                    }
                    model: comboBox1Model;
                    text: comboBox1Model.get(0).name;
                }
                Item{width: 100; height: parent.height;}
                CheckBox {
                    id:markIdCheckBox
                    font{ family: localFont.name; pointSize: 10; }
                    color:"#333333"
                    text: "关"
                    active: true;
                    visible: false;
                }
            }
        }
        Item{width: parent.width; height: 60;}
        Item{
            width: parent.width;height: 32;
            Row{
                width: 140;anchors.right: parent.right;spacing: 4;
                PushButtons {
                    width: 60;  height: 23;
                    label: qsTr("确定")
                    onClicked: { RadarAfterglowModel.dealAfterglow(markIdCheckBox.isChecked, comboBox1Model.get(timeComboBox.indexs).idx)}
                }
                PushButtons {
                    width: 60;  height: 23;
                    label: qsTr("取消")
                    onClicked: { RadarAfterglowModel.closeDialog() }
                }
            }
        }
    }

    Connections {
        target: RadarAfterglowModel;
        onSetRadarVideoKeepMinutes: {
            timeComboBox.indexs = minutes;
            timeComboBox.text = timeComboBox.model.get(timeComboBox.indexs).name;
        }
        }
}
