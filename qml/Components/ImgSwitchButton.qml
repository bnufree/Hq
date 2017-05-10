import QtQuick 1.1

Rectangle {
    id: roots
    width: 8; height: 8

    property string firstImg
    property string secondImg
    property bool checked: false
    property bool checkable: true

    onCheckedChanged: {
        if ( roots.checked )
        {
            img.source = roots.secondImg;
        }
        else
        {
            img.source = roots.firstImg;
        }
    }

    Image {
        id: img
        anchors.centerIn: parent
        source: roots.firstImg
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if ( checkable )
            {
                roots.checked = !roots.checked
            }
        }
    }
}
