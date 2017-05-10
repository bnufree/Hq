import QtQuick 1.1
import "Components"

Rectangle {
    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int rotationvalue: 0;
    property int rotation: 0;
    property string mm_longitude:"0";
    property string mm_latitude:"0";
    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column{
                anchors.fill: parent; anchors.margins: 10; spacing: 8;
                /*-------------------------------------------
                * 纬度
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 40;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("经度:");
                    }
                    TextEdit {
                        id: longitudeText;
                        width: 160; height: 23; color: "#8e8e8e";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 11; }
                        smooth: true; text: qsTr("");
                    }
                    Item{ width: 20; height: parent.height; }
                    Text {
                        width: 40;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("纬度:");
                    }
                    TextEdit {
                        id: latitudeText;
                        width: 160; height: 23; color: "#8e8e8e";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 11; }
                        smooth: true; text: qsTr("");
                    }
                }

                /*-------------------------------------------
                * 纬度
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 40;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("名称:");
                    }
                    LineEdit{
                        id: nameText;
                        width: 381; height: 23; color: "#1a1a1a";
                        KeyNavigation.tab: groupText;
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                    Item{ width: 20; height: parent.height; }
                    Text {
                        width: 40;  height: 23; visible: false;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("分组:");
                    }
                    LineEdit{
                        id: groupText;
                        visible: false;
                        width: 160; height: 23; color: "#1a1a1a";
                        KeyNavigation.tab: markedText;
                        validator: DoubleValidator{ bottom: -90.0; top: 90.0; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * 备注
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 40;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("备注:");
                    }
                    Rectangle{
                        width: 381; height: 63;
                        border { width: 1; color:"#1a1a1a";} clip: true;
                        TextEdit {
                            id: markedText;
                            anchors.fill: parent; color: "#1a1a1a"; anchors.margins: 3;
                            selectByMouse: true; readOnly: false;
                            font{ family: localFont.name; pointSize: 11; }
                            smooth: true; text: qsTr("");

                        }
                    }
                }
                Item{ width: parent.width; height: 6; }
                Row{
                    anchors.right: parent.right; anchors.rightMargin: 6; spacing: 10;
                    PushButtons {
                        width: 70;  height: 23;
                        label: qsTr("保存");
                        onClicked: {
                            if(nameText.text != "" )
                            {
//                                MarkeLocationModel.sendSave( longitudeText.text, latitudeText.text, nameText.text, groupText.text, markedText.text, mm_longitude,  mm_latitude);
                                MarkeLocationModel.sendSave(mm_longitude,  mm_latitude, nameText.text, groupText.text, markedText.text);
                            }else
                            {

                            }

                            MarkeLocationModel.dealClose();
                        }
                    }
                    PushButtons {
                        width: 70;  height: 23;
                        label: qsTr("取消")
                        onClicked: MarkeLocationModel.dealClose();
                    }
                }

                Connections{ target:MarkeLocationModel ; onSendClearMarke: { nameText.text = ""; markedText.text = ""; groupText.text = ""; } }
                Connections{ target:MarkeLocationModel ; onSendClearName: { nameText.text = ""; } }
                Connections{ target: MarkeLocationModel; onSendMarkeLocation: { longitudeText.text = longitude; latitudeText.text = latitude;
                            mm_longitude = m_longitude;
                            mm_latitude = m_latitude;  }


                }
            }
        }
    }
}
