import QtQuick 1.1

Rectangle {
    id: dateTimeBox
    width: 233; height: 27; radius: 1;
    border{ width: 1; color: "#c2c2c2"; }
    property bool readOnly: false
    property alias maxDay: dayIntValidator.top;
    property bool datevisible: true;
    property bool timevisible: true;
    property bool monthvisible: true;
    property bool dayvisible: true;
    property string dateTime: "1970-1-1 12:12:12"
    property int fontPointSize: 10
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property string getyear: yearInput.text;
    property string getmonth: monthInput.text;
    property string getday:dayInput.text;
    property string gethour: hourInput.text;
    property string getminute: minInput.text;
    property string getsecond: secInput.text;
    signal sendDateTimeChanged();
    onDateTimeChanged: {
        var l_dateTime = String(dateTime);
        var l_dateTimeArray = new Array();
        l_dateTimeArray = l_dateTime.split(" ");

        //日期
        var l_date = l_dateTimeArray[0]
        var l_dateArray = new Array();
        l_dateArray = l_date.split("-");

        //时间
        var l_time = l_dateTimeArray[1]
        var l_timeArray = new Array();
        l_timeArray = l_time.split(":");
        setDateTime(l_dateArray[0], l_dateArray[1], l_dateArray[2], l_timeArray[0], l_timeArray[1], l_timeArray[2] )
    }
    /*-------------------------------------------
     * 设置日期时间
     ---------------------------------------------*/
    function setDateTime(year, month, day, hour, min, sec) {
        yearInput.text = year;
        monthInput.text = month;
        dayInput.text = day;

        hourInput.text = hour;
        minInput.text = min;
        secInput.text = sec;
    }
    /*-------------------------------------------
     * 获得日期时间 格式"1970-01-01 12:12:12"
     ---------------------------------------------*/
    function getDateTime() {
        //非法值判断
        var l_year = Math.max(parseInt(yearInput.text, 10), 1900)

        var l_month = Math.max(parseInt(monthInput.text, 10), 1)
        monthInput.text = l_month;

        var l_day = Math.max(parseInt(dayInput.text, 10), 1)
        dayInput.text = l_day;

        return l_year + "-" + l_month + "-" + l_day + " " + hourInput.text + ":" + minInput.text + ":" + secInput.text;
    }
    /*-------------------------------------------
     * 根据“大小月份”，“瑞年”等判断日期的限定值
     ---------------------------------------------*/
    function maxDayCheck() {
        var monthValue = monthInput.text;
        if ( monthValue == "2"||monthValue == "02" )
        {
            if ( parseInt(yearInput.text, 10)%4 == 0 ) {
                dayIntValidator.top = 29;
                if(dayInput.text > 29)
                {
                    dayInput.text = 29;
                }
            } else {
                dayIntValidator.top = 28;
                if(dayInput.text > 28)
                {
                    dayInput.text = 28;
                }
            }
        }
        else if ( monthValue == "4" || monthValue == "6" || monthValue == "9" || monthValue == "11"||
                 monthValue == "04" || monthValue == "06" || monthValue == "09"  )
        {
            dayIntValidator.top = 30;
            if(dayInput.text > 30)
            {
                dayInput.text = 30;
            }
        }
        else
        {
            dayIntValidator.top = 31;
            if(dayInput.text > 31)
            {
                dayInput.text = 31;
            }
        }
    }
    /*-------------------------------------------
     * 年月日 组
     ---------------------------------------------*/
    Row {
        id: dateRow
        height: parent.height; spacing: 1;
        anchors.verticalCenter: parent.verticalCenter
        anchors {left: parent.left; leftMargin: 4;}
        /*-------------------------------------------
         * 年
         ---------------------------------------------*/
        TextInput {
            id: yearInput
            anchors.verticalCenter: parent.verticalCenter
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            focus: true
            color: readOnly ? "#4f4f4f" : "black";
            text: "1990"; readOnly: dateTimeBox.readOnly;
            validator: IntValidator{bottom: 1900; top: 2099;}
            maximumLength:4
            KeyNavigation.tab: monthInput;
            onFocusChanged:{ if(focus) selectAll();
            }
            onTextChanged: {
                maxDayCheck();
//                var yearValue = yearInput.text;
//                var count = yearValue.length;
//                if(yearInput.text == "")
//                {
//                    yearInput.text = Qt.formatDateTime(new Date(),"yyyy");
//                }
//                if(count > 1 && (yearValue[0] == 0))
//                {
//                    yearInput.text = Qt.formatDateTime(new Date(),"yyyy");
//                }
                dateTimeBox.sendDateTimeChanged();
            }
//            Keys.onPressed: {
//                if ( event.key == Qt.Key_Up )
//                {
//                    var l_year = Math.min( parseInt(yearInput.text, 10) + 1, 2099 );
//                    yearInput.text = Math.max( l_year, 1900 )
//                }
//                else if ( event.key == Qt.Key_Down )
//                {
//                    yearInput.text = Math.max( parseInt(yearInput.text, 10) - 1, 1900 )
//                }
//                else if ( event.key == Qt.Key_Right && yearInput.cursorPosition == yearInput.text.length )
//                {
//                    monthInput.focus = true
//                }
//            }
            Keys.onUpPressed: year = year+1;
            Keys.onDownPressed: year = year-1;
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            color: readOnly ? "#4f4f4f" : "black"; text: qsTr("-")
            visible: monthvisible;
        }
        /*-------------------------------------------
         * 月
         ---------------------------------------------*/
        TextInput {
            id: monthInput
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            readOnly: dateTimeBox.readOnly; text: "12";
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            focus: true
            color: readOnly ? "#4f4f4f" : "black"
            validator: IntValidator{bottom: 1; top: 12;}
            maximumLength:2
            KeyNavigation.tab: dayInput;
            onFocusChanged:{ if(focus) selectAll();
                   maxDayCheck();
            }
            visible: monthvisible;
//            Keys.onPressed: {
//                if ( event.key == Qt.Key_Up )
//                {
//                    monthInput.text = Math.min( parseInt(monthInput.text, 10) + 1, 12 )
//                }
//                else if ( event.key == Qt.Key_Down )
//                {
//                    monthInput.text = Math.max( parseInt(monthInput.text, 10) - 1, 1 )
//                }
//                else if ( event.key == Qt.Key_Left && monthInput.cursorPosition == 0 )
//                {
//                    yearInput.focus = true
//                }
//                else if ( event.key == Qt.Key_Right && monthInput.cursorPosition == monthInput.text.length )
//                {
//                    dayInput.focus = true
//                }
//            }
            Keys.onUpPressed: ((month+1)%13 > 0) ? month = month+1 : month = 1;
            Keys.onDownPressed: month > 1 ? month = month-1 : month = 12;
            onTextChanged: {
//                maxDayCheck();
//                var monthValue = monthInput.text;
//                if(monthInput.text == "")
//                {
//                    monthInput.text = "1";
//                }
//                if((monthValue[0] == 0))
//                {
//                    monthInput.text = Qt.formatDateTime(new Date(),"M");
//                }
                dateTimeBox.sendDateTimeChanged();
            }

        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("-")
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            color: readOnly ? "#4f4f4f" : "black";
            visible: dayvisible;
        }
        /*-------------------------------------------
         * 日
         ---------------------------------------------*/
        TextInput {
            id: dayInput
            anchors.verticalCenter: parent.verticalCenter
            text: "6"
            readOnly: dateTimeBox.readOnly
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            focus: true
            color: readOnly ? "#4f4f4f" : "black"
            validator: dayIntValidator;
            maximumLength:2
            KeyNavigation.tab: hourInput;
            onFocusChanged:{ if(focus) selectAll();
                maxDayCheck();
            }
            onTextChanged: {
                if(dayInput.text == "")
                {
                    dayInput.text = "1";
                }

                var dayInputValue = monthInput.text;
                if((dayInputValue[0] == 0))
                {
                    dayInput.text = Qt.formatDateTime(new Date(),"d");
                }

                dateTimeBox.sendDateTimeChanged();
//                if(Math.abs(dayInput.text) == 0)
//                {
//                    dayInput.text = 1;
//                }
            }
            visible: dayvisible;
            Keys.onUpPressed: ((day+1)%32 > 0) ? day = day+1 : day = 1;
            Keys.onDownPressed: day > 1 ? day = day-1 : day = 31;
        }
    }
    /*-------------------------------------------
     * 日 的验证器
     ---------------------------------------------*/
    IntValidator{ id: dayIntValidator; bottom: 1; top: 31; }
    /*-------------------------------------------
     * 时分秒 组
     ---------------------------------------------*/
    Row {
        id: timeRow;
        spacing: 2;
        anchors.verticalCenter: parent.verticalCenter;
        anchors {left: dateRow.right; leftMargin: 4}
        visible: timevisible;
        //时
        TextInput {
            id: hourInput
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            text: "12"
            readOnly: dateTimeBox.readOnly
            focus: true
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            color: readOnly ? "#4f4f4f" : "black"
            validator: IntValidator{bottom: 0; top: 23;}
            maximumLength:2
            KeyNavigation.tab: minInput;
            onFocusChanged: {if(focus) selectAll();
            }
            onTextChanged: {
//                if(hourInput.text == "")
//                {
//                    hourInput.text = "0";
//                }
                dateTimeBox.sendDateTimeChanged();
//                if(Math.abs(hourInput.text) == 0)
//                {
//                    hourInput.text = 0;
//                }

            }
            Keys.onUpPressed: ((hour+1)%24 > 0) ? hour = hour+1 : hour = 1;
            Keys.onDownPressed: hour > 1 ? hour = hour-1 : hour = 23;
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr(":")
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            color: readOnly ? "#4f4f4f" : "black"
        }
        //分
        TextInput {
            id: minInput
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            text: "12"
            readOnly: dateTimeBox.readOnly
            focus: true
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            color: readOnly ? "#4f4f4f" : "black"
            validator: IntValidator{bottom: 0; top: 59;}
            maximumLength:2
            KeyNavigation.tab: secInput;
            onFocusChanged:{ if(focus) selectAll();
            }
            onTextChanged: {
//                if(minInput.text == "")
//                {
//                    minInput.text = "0";
//                }
                dateTimeBox.sendDateTimeChanged();
//                if(Math.abs(minInput.text) == 0)
//                {
//                    minInput.text = 0;
//                }
            }
            Keys.onUpPressed: ((minute+1)%60 > 0) ? minute = minute+1 : minute = 1;
            Keys.onDownPressed: minute > 1 ? minute = minute-1 : minute = 59;
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr(":")
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            color: readOnly ? "#4f4f4f" : "black"
        }
        //秒
        TextInput {
            id: secInput
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            text: "12"
            readOnly: dateTimeBox.readOnly
            focus: true
            font{ family: localFont.name; pointSize: dateTimeBox.fontPointSize; }
            selectByMouse: true
            color: readOnly ? "#4f4f4f" : "black"
            validator: IntValidator{bottom: 0; top: 59;}
            maximumLength:2
            onFocusChanged:{ if(focus) selectAll();
            }
            KeyNavigation.tab: yearInput;
            onTextChanged: {
//                if(secInput.text == "")
//                {
//                    secInput.text = "0";
//                }
                dateTimeBox.sendDateTimeChanged();
//                if(Math.abs(secInput.text) == 0)
//                {
//                    secInput.text = 0;
//                }
            }
            Keys.onUpPressed: ((second+1)%60 > 0) ? second = second+1 : second = 1;
            Keys.onDownPressed: second > 1 ? second = second-1 : second = 59;
        }

    }
}
