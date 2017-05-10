import QtQuick 1.1

Item {
    id: checkBox;
    property alias text: textItem.text;
    property bool isChecked: false;
    property bool active: false;
    signal checked(bool chk);
    property alias font: textItem.font
    property alias color: textItem.color
    width: 160
    height: 24

    Row {
        spacing: 8;
        anchors.fill: parent;
        BorderImage {
            id: img
            width: 16; height: 16;
            anchors.verticalCenter: parent.verticalCenter;
            source: isChecked ? "qrc:/systemsmanage/qml/icons/check.png" : "qrc:/systemsmanage/qml/icons/uncheck.png"
            smooth: true
            horizontalTileMode: BorderImage.Stretch;
            verticalTileMode: BorderImage.Stretch;
        }
        Text {
            id: textItem
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            font{ family: localFont.name; pointSize: 12; /*bold: true*/ }
            color: checkBox.active ? "black" : "#808080"
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if ( active )
            {
                isChecked = !isChecked;
                checkBox.checked( isChecked )
            }
        }
    }
}
