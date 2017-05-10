import QtQuick 1.1

Rectangle {
    id: lineEdit;
    signal textChange( string text );
    property alias text: textInputs.text;
    property alias validator: textInputs.validator;
    property alias readOnly: textInputs.readOnly;
    width: 153; height: 24; clip: true; color: "#A1A1A1"; radius: 1;
    Rectangle {
        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2"; radius: parent.radius;
        Rectangle {
            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff"; radius: parent.radius;
            TextInput {
                id: textInputs;
                width: parent.width;
                anchors {left: parent.left; leftMargin: 4}
                anchors.verticalCenter: parent.verticalCenter
                selectByMouse: true; text: ""; focus: true;
                font{ family: localFont.name; pointSize: 10; }
                onTextChanged: { lineEdit.textChange(textInputs.text); }
                onFocusChanged: if(focus) selectAll();
            }
        }
    }
    onFocusChanged: textInputs.focus = true;
}
