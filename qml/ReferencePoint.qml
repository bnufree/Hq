import QtQuick 1.1
import "Components"

Rectangle {
    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int referenceidx: referenceCompo.idxx;
    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column{
                anchors.fill: parent; anchors.margins: 10; spacing: 13;
                /*-------------------------------------------
                * 经度
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("经   度:");
                    }
                    LineEdit{
                        id: longitudeText;
                        width: 229; height: 23;
                        KeyNavigation.tab: latitudeText;
                        validator: DoubleValidator{ bottom: -180.0; top: 180.0; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * 纬度
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("纬   度:");
                    }
                    LineEdit{
                        id: latitudeText;
                        width: 229; height: 23;
                        KeyNavigation.tab: referenceCompo;
                        validator: DoubleValidator{ bottom: -90.0; top: 90.0; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * 参考点
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("参考点:");
                    } z: 1;
                    ComboBox{
                        id: referenceCompo;
                        width: 229; height: 23;
                        KeyNavigation.tab: xText;
                        color: "#ffffff";
                        border { width: 1; color:"#828282";}
                        model: referenceModel;
                        ListModel { id: referenceModel; }
                        Connections{ target: ReferenceModel; onSendClearCombosss: referenceModel.clear(); }
                        Connections{
                            target: ReferenceModel;
                            onSendAddReferenceList: {
                                referenceModel.append({"idx": idx,"name": reference});
                                referenceCompo.text = referenceModel.get(0).name;
                                referenceCompo.idxx = referenceModel.get(0).idx;
                            }
                        }
                        onSendIdx: {var currentText = referenceCompo.text;
                            if(currentText != "")
                            {
                                console.log("*******currentText:",currentText);
                                ReferenceModel.sendReferenceId(idx);
                            }
                        }
                    }
                }
                /*-------------------------------------------
                * x
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("X:");
                    }
                    LineEdit{
                        id: xText;
                        width: 229; height: 23;
                        KeyNavigation.tab: yText;
                        validator: IntValidator{ bottom: 0; top: 100000; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * y
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("Y:");
                    }
                    LineEdit{
                        id: yText;
                        width: 229; height: 23;
                        KeyNavigation.tab: azimuthText;
                        validator: IntValidator{ bottom: 0; top: 100000; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * 方位
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("方   位:");
                    }
                    LineEdit{
                        id: azimuthText;
                        width: 229; height: 23;
                        KeyNavigation.tab: distanceText;
                        validator: DoubleValidator{ bottom: -360.0; top: 360.0; }
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }
                /*-------------------------------------------
                * 距离
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("距   离:");
                    }
                    LineEdit{
                        id: distanceText;
                        width: 229; height: 23;
                        KeyNavigation.tab: nameText;
                        validator: DoubleValidator{ bottom: -1000000.0; top: 1000000.0; }
                    }
                }
                /*-------------------------------------------
                * 名称
                ---------------------------------------------*/
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("名   称:");
                    }
                    LineEdit{
                        id: nameText;
                        width: 229; height: 23;
                    }
                }
                Row{
                    Text {
                        width: 50;  height: 23;
                        font{ family: localFont.name; pointSize: 11; }
                        color: "#1a1a1a"; text: qsTr("海   图:");
                    }
                    PushButtons {
                        width: 70;  height: 23;
                        label: qsTr("居中")
                        onClicked: ReferenceModel.sendCenter(latitudeText.text,longitudeText.text);
                    }
                }

                Row{
                    anchors.right: parent.right; anchors.rightMargin: 6; spacing: 6;
                    /*-------------------------------------------
                     * 错误提示信息
                     ---------------------------------------------*/
                    Rectangle {
                        id: errorButton;
                        width: 120; height: 23; color: "red"; radius: 2; clip: true; opacity: 0;
                        Text {
                            id: errorinfoText;
                            anchors.centerIn: parent;
                            color: "#FAFAFA"; smooth: true;
                            font{ family: localFont.name; pointSize: 10; bold: true;}
                        }
                        SequentialAnimation {
                            id: errorParallel;
                            running: false;
                            NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 1000 }
                            NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 2000 }
                            NumberAnimation { target: errorButton; property: "opacity"; to: 0; duration: 1000 }
                        }
                    }
                    PushButtons {
                        width: 70;  height: 23;
                        label: qsTr("记录")
                        onClicked: {
                            if(nameText.text != "")
                            {
                                ReferenceModel.sendSave(latitudeText.text, longitudeText.text, nameText.text );
                            }else{
                                errorParallel.running = true;
                                nameText.focus = true;
                                errorinfoText.text = "名称不能为空.";
                            }
                        }
                    }
                    PushButtons {
                        width: 70;  height: 23;
                        label: qsTr("取消")
                        onClicked: ReferenceModel.dealClose();
                    }
                }
            }
            Connections{
                target: ReferenceModel;
                onSendLatitudeAndLongitude: {
                    latitudeText.text = latitude;
                    longitudeText.text = longitude;
                }
            }

            Connections{
                target: ReferenceModel;
                onSendSetReferenceCompo: {
                    referenceCompo.indexs = idx;
                }
            }


            Connections{
                target: ReferenceModel;
                onSendReferenceInfo: {
                    xText.text = x;
                    yText.text = y;
                    azimuthText.text = azimuth;
                    distanceText.text = distance;
                    nameText.text = name;
                }
            }
        }
    }
}
