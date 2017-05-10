import QtQuick 1.1

Rectangle {
    id: comboBox
    width: 105; height: 23; color: "#e2e2e2"; radius: 1;
    property variant items: ["全区域"];
    property alias indexs: comboBoxLiset.currentIndex;
    property alias text: chosenItemText.text;
    property alias model: comboBoxLiset.model;
    property bool isEnabled: true;
    property int idxx: 0;
    signal sendIndex( int idx );
    signal sendIdx( int idx );
    Row{
        width: parent.width; height: parent.height; clip: true;
        /*-------------------------------------------
         * 文本框
         ---------------------------------------------*/
        Item{
            width: parent.width-arrowButton.width; height: parent.height; anchors.verticalCenter: parent.verticalCenter; clip: true;
            Text {
                id: chosenItemText;
                anchors.fill: parent; anchors.leftMargin: 4;
                anchors.verticalCenter: parent.verticalCenter;
                color: isEnabled ? "black" : "#4F4F4F";
                font{ family: localFont.name; pointSize: 10; bold: false; }
                onFocusChanged: {
                    if(!focus)
                        comboBox.state = "";
                }
            }
        }
        Item { width: 2; height: parent.height; }
        BorderImage {
            id: arrowButton;
            width: 24; height: parent.height-2;
            anchors.verticalCenter: parent.verticalCenter;
            border { left: 1; top: 1; right: 1; bottom: 1 }
            horizontalTileMode: BorderImage.Stretch;
            verticalTileMode: BorderImage.Stretch;
            source: "qrc:/ecdisviewer/qml/images/commboxarrow.png";
        }
    }
    /*-------------------------------------------
     * 下拉框
     ---------------------------------------------*/
    Rectangle {
        id: dropDownRect; width: parent.width; height: 0;
        anchors{ left: comboBox.left; top: comboBox.bottom; topMargin: 1; } clip: true; color: "#ffffff";
        ListView {
            id: comboBoxLiset
            anchors.fill: parent;
            delegate: comboDelegate;
            currentIndex: 0;
            Component {
                id: comboDelegate;
                Rectangle {
                    id: comboItemRect; width: dropDownRect.width; height: 20; color: listMouseArea.containsMouse ? "#b0efff" : "#eeeeee";
                    Text {
                        id: comboItemText;
                        anchors{ fill: parent; leftMargin: 5; verticalCenter: parent.verticalCenter; }
                        color: "black"; text: qsTr( name );
                        verticalAlignment: Text.AlignVCenter
                        MouseArea {
                            id: listMouseArea;
                            anchors.fill: parent;
                            onClicked: { comboBox.state = ""; chosenItemText.text = name; comboBoxLiset.currentIndex = index; comboBox.sendIndex(index); comboBox.sendIdx(idx); idxx = idx; }
                            hoverEnabled: true;
                        }
                    }
                }
            }
        }
    }
    MouseArea {
        anchors.fill: parent; onClicked: {
            if(isEnabled) comboBox.state = ( (comboBox.state == "dropDown") ? "" : "dropDown" );
            chosenItemText.focus = true;
        }
    }
    states: State { name: "dropDown"; PropertyChanges { target: dropDownRect; height: (comboBoxLiset.count > 5) ? 120 : comboBoxLiset.count*20; } }
    transitions: Transition { NumberAnimation { target: dropDownRect; properties: "height"; easing.type: Easing.OutExpo; duration: 300 } }
}
