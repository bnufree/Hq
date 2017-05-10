import QtQuick 1.1

Item {
    id: pushButton; width: 58; height: 23;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property string label: "Button";
    property string backgroundColor:"#005eaf";
    signal clicked();
    Rectangle {
        anchors.fill: parent; radius: 2; smooth: true;
        color: buttonMouseArea.containsMouse ? "#ff8200" : backgroundColor;
        Text {
            anchors.centerIn: parent;
            color: "white"; text: label;
            font{ family: localFont.name; pointSize: 10; }
        }

    }
    MouseArea{
        id: buttonMouseArea;
        anchors.fill: parent;
        hoverEnabled: false;
        onClicked: pushButton.clicked();
    }
}
