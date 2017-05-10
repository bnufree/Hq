import QtQuick 1.1

Rectangle {
    id: dateTimeEdit
    width: 153; height: 24; clip: true; color: "#616161"; anchors.verticalCenter: parent.verticalCenter;
    property int year: 2014;
    property int month: 06;
    property int day: 20;
    property int hour: 15;
    property int minute: 30;
    property int second: 01;
    property string dateTime: year+"-"+month+"-"+day+" "+hour+":"+minute+":"+second;
    signal dateTimeChanged( string dateTime );
    Rectangle {
        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
        Rectangle {
            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
            Row{
                anchors.fill: parent;
                Item{ width: 3; height: parent.height; }
                TextInput{
                    id: yearText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: year;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 1800; top: 9999; }
                    KeyNavigation.tab: monthText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: year = year+1;
                    Keys.onDownPressed: year = year-1;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
                TextInput{
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: "-"; readOnly: true;
                    font{ family: localFont.name; pointSize: 10; }
                }
                TextInput{
                    id: monthText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: (month%13 > 9) ? month%13 : "0"+month%13;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 1; top: 12; }
                    KeyNavigation.tab: dayText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: ((month+1)%13 > 0) ? month = month+1 : month = 1;
                    Keys.onDownPressed: month > 1 ? month = month-1 : month = 12;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
                TextInput{
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: "-"; readOnly: true;
                    font{ family: localFont.name; pointSize: 10; }
                }
                TextInput{
                    id: dayText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: (day%32 > 9) ? day%32 : "0"+day%32;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 1; top: 31; }
                    KeyNavigation.tab: hourText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: ((day+1)%32 > 0) ? day = day+1 : day = 1;
                    Keys.onDownPressed: day > 1 ? day = day-1 : day = 31;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
                Item{ width: 6; height: parent.height; }

                TextInput{
                    id: hourText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: (hour%24 > 9) ? hour%24 : "0"+hour%24;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 0; top: 23; }
                    KeyNavigation.tab: minText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: ((hour+1)%24 > 0) ? hour = hour+1 : hour = 1;
                    Keys.onDownPressed: hour > 1 ? hour = hour-1 : hour = 23;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
                TextInput{
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: ":"; readOnly: true;
                    font{ family: localFont.name; pointSize: 10; }
                }
                TextInput{
                    id: minText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: (minute%60 > 9) ? minute%60 : "0"+minute%60;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 0; top: 59; }
                    KeyNavigation.tab: secondText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: ((minute+1)%60 > 0) ? minute = minute+1 : minute = 1;
                    Keys.onDownPressed: minute > 1 ? minute = minute-1 : minute = 59;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
                TextInput{
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: ":"; readOnly: true;
                    font{ family: localFont.name; pointSize: 10; }
                }
                TextInput{
                    id: secondText;
                    anchors.verticalCenter: parent.verticalCenter;
                    selectByMouse: true; text: (second%60 > 9) ? second%60 : "0"+second%60;
                    font{ family: localFont.name; pointSize: 10; }
                    validator: IntValidator{ bottom: 0; top: 59; }
                    KeyNavigation.tab: yearText;
                    onFocusChanged: if(focus) selectAll();
                    Keys.onUpPressed: ((second+1)%60 > 0) ? second = second+1 : second = 1;
                    Keys.onDownPressed: second > 1 ? second = second-1 : second = 59;
                    onTextChanged: dateTimeEdit.dateTimeChanged(dateTime);
                }
            }
        }
    }
}
