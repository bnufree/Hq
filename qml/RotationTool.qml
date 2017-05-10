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
            Row{
                anchors.fill: parent;
                Item{ width: 20; height: parent.height; }

                Rectangle{
                    width: 45; height: 45; radius: 45; color: Qt.rgba(0.88,0.88,0.88,0.5 ); smooth: true;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.verticalCenterOffset:  -6;
                    Rectangle{
                        anchors.fill: parent; anchors.margins: 3; radius: 45; color: leftRotationMouse.containsMouse ? "#ffffff" : "#ff8200"; smooth: true;
                        BorderImage {
                            id: arrowButton;
                            width: 27; height: 22; smooth: true;
                            anchors.centerIn: parent;
                            border { left: 1; top: 1; right: 1; bottom: 1 }
                            horizontalTileMode: BorderImage.Stretch;
                            verticalTileMode: BorderImage.Stretch;
                            source: leftRotationMouse.containsMouse ? "qrc:/ecdisviewer/qml/images/rtationblack.png" : "qrc:/ecdisviewer/qml/images/rtationwhite.png";
                        }
                        MouseArea{
                            id: leftRotationMouse; anchors.fill: parent;
                            onClicked: {
                                rotation = (rotation - 90)%360;
                                RotationModel.sendRotationValue( rotationvalue +rotation );
                            }
                        }
                    }
                }
                Item{ width: 15; height: parent.height; }
                Rectangle{
                    width: 45; height: 45; radius: 45; color: Qt.rgba(0.88,0.88,0.88,0.5 ); smooth: true;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.verticalCenterOffset:  -6;
                    Rectangle{
                        anchors.fill: parent; anchors.margins: 3; radius: 45; color: rightRotationMouse.containsMouse ? "#ffffff" : "#ff8200"; smooth: true;
                        BorderImage {
                            width: 27; height: 22; smooth: true;
                            anchors.centerIn: parent;
                            anchors.horizontalCenterOffset: 4;
                            border { left: 1; top: 1; right: 1; bottom: 1 }
                            horizontalTileMode: BorderImage.Stretch;
                            verticalTileMode: BorderImage.Stretch;
                            source: rightRotationMouse.containsMouse ? "qrc:/ecdisviewer/qml/images/rtationblack_l.png" : "qrc:/ecdisviewer/qml/images/rtationwhite_l.png";
                        }
                        MouseArea{
                            id: rightRotationMouse; anchors.fill: parent;
                            onClicked: {
                                rotation = (rotation + 90)%360;
                                RotationModel.sendRotationValue( rotationvalue +rotation );
                            }
                        }
                    }
                }
                Item{ width: 15; height: parent.height; }
                LineEdit{
                    id: rotationvalueText;
                    width: 148; height: 32;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.verticalCenterOffset:  -6;
                    validator: IntValidator{ bottom: -360; top: 360; }
                    color: "#000000"; text: "0";
                }

                Item{ width: 10; height: parent.height; }

                PushButtons{
                    width: 75; height: 28;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.verticalCenterOffset:  -6;
                    label: qsTr("确定");
                    onClicked: {
                        rotationvalue = rotationvalue+ Math.max(rotationvalueText.text,-1000);
                        RotationModel.sendRotationValue( rotationvalue );
                    }
                }

                Item{ width: 15; height: parent.height; }

                PushButtons{
                    width: 75; height: 28;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.verticalCenterOffset:  -6;
                    label: qsTr("重置");
                    onClicked: RotationModel.sendRotationValue( 0 );
                }
            }
        }
    }
}
