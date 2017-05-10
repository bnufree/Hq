import QtQuick 1.1
import "Components"


Rectangle {
    id:rootrect;
    property string errorinfo: "No Error";
    property alias errors: errorParallel.running;

    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int spinBoxState: 0;
    function getTime(year, month, day, hour, min, sec)//messageBox
    {
        if(year == "" || month == "" || day == "" || hour == "" || min == "" || sec == ""
            ||  month == "00" || day == "00"||  month == "0" || day == "0" || year == "00"
            ||  year == "0" || year == "000"|| year == "0000")
        {
           return "";
        }
        else{
            var n_date = "";
            n_date += year + "-";
            n_date += ((month%13 > 9) ? month%13 : "0"+month%13) + "-";
            n_date += (day%32 > 9) ? day%32 : "0"+day%32;
            n_date += " "
            n_date += ((hour%24 > 9) ? hour%24 :( "0" + hour%24)) + ":";
            n_date += ((min%60 > 9) ? min%60 : ("0" + min%60)) + ":";
            n_date += (sec%60 > 9) ? sec%60 : ("0" + sec%60);
            return n_date;
        }
    }
    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column {
                anchors.fill: parent;  spacing: 0; anchors.margins: 0;
                Rectangle {
                    width: parent.width; height: parent.height-2-buttonRect.height; color: "#F5F5F5";
                    Column{
                        anchors{ fill: parent; margins: 10; }
                        Row{
                            spacing: 6;
                            /*-------------------------------------------
                             *
                             ---------------------------------------------*/
                            Rectangle{
                                width: 14; height: 14; color: "#cccccc"; radius: 14;
                                anchors.verticalCenter: parent.verticalCenter; smooth: true;
                                Rectangle{
                                    anchors.centerIn: parent;
                                    width: 8; height: 8; color: "#1a1a1a"; radius: 8;
                                    smooth: true; visible: spinBoxState==0 ? true : false;
                                }
                                MouseArea{ anchors.fill: parent; onClicked: spinBoxState = 0; }
                            }
                            Text {
                                width: 28; height: 22;
                                color: "#1a1a1a"; smooth: true; text: qsTr("当前时间");
                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                MouseArea{ anchors.fill: parent; onClicked: spinBoxState = 0; }
                            }
                        }
                        Item{ width: parent.width; height: 10; }
                        /*-------------------------------------------
                         * 前置条件
                         ---------------------------------------------*/
                        Row{
                            width: 190; height: 22; spacing: 3;
                            Item{ width: 28; height: 22; }
                            Text {
                                width: 28; height: 22;
                                color: "#1a1a1a"; smooth: true; text: qsTr("前置");
                                font{ family: localFont.name; pointSize: 10; bold: false; }
                            }
                            LineEdit{
                                id: hourEdit;
                                width: 60; height: 22; text: "2";
                                onTextChange: trackShowModel.dealUpdateAddHour( text );
                                validator: IntValidator{ bottom: 0; top: 24; }
                                KeyNavigation.tab: startDateTime;
                            }
                            Text {
                                width: 28; height: 22;
                                color: "#1a1a1a"; smooth: true; text: qsTr("小时");
                                font{ family: localFont.name; pointSize: 10; bold: false; }
                            }
                        }
                        Item{ width: parent.width; height: 8; }
                        Row{
                            spacing: 6;
                            /*-------------------------------------------
                             *
                             ---------------------------------------------*/
                            Rectangle{
                                width: 14; height: 14; color: "#cccccc"; radius: 14;
                                anchors.verticalCenter: parent.verticalCenter; smooth: true;
                                Rectangle{
                                    anchors.centerIn: parent;
                                    width: 8; height: 8; color: "#1a1a1a"; radius: 8;
                                    smooth: true; visible: spinBoxState==1 ? true : false;
                                }
                                MouseArea{ anchors.fill: parent; onClicked: spinBoxState = 1; }
                            }
                            Text {
                                width: 28; height: 22;
                                color: "#1a1a1a"; smooth: true; text: qsTr("自定义时间");
                                font{ family: localFont.name; pointSize: 10; bold: false; }
                                MouseArea{ anchors.fill: parent; onClicked: spinBoxState = 1; }
                            }
                        }
                        Item{ width: parent.width; height: 8; }
                        Row{
                            spacing: 20;
                            Item{ width: 8; height: 22; }
                            /*-------------------------------------------
                             * 开始时间
                             ---------------------------------------------*/
                            Row {
                                width: 190; height: 22; spacing: 3;
                                Text {
                                    width: 28; height: 22;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("开始");
                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                }

                                DateTimeBox {
                                    id: startDateTime;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    width: 153; height: 24; color: "#ffffff"; fontPointSize: 9;
                                    dateTime: trackShowModel.getDateTime(2);
                                }

                            }
                            /*-------------------------------------------
                             * 结束时间
                             ---------------------------------------------*/
                            Row{
                                width: 190; height: 22; spacing: 3;
                                Text {
                                    width: 28; height: 22;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("结束");
                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                }
                                DateTimeBox {
                                    id: endDateTime;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    width: 153; height: 24; color: "#ffffff"; fontPointSize: 9;
                                    dateTime: trackShowModel.getDateTime(0);
                                }
                            }
                        }
                    }
                }
                /*-------------------------------------------
                 * 按钮区域
                 ---------------------------------------------*/
                Rectangle {
                    id: buttonRect; width: parent.width; height: 48; color: "#F1F1F1";
                    Row{
                        anchors.right: parent.right; anchors.rightMargin: 20; anchors.verticalCenter: parent.verticalCenter; spacing: 18;
                        /*-------------------------------------------
                         * 错误提示信息
                         ---------------------------------------------*/
                        Rectangle {
                            id: errorButton; width: 260; height: 26; color: "red"; radius: 2; clip: true; opacity: 0;
                            Text {
                                id: errorinfoText;
                                anchors.centerIn: parent;
                                color: "#FAFAFA"; smooth: true;
                                font{ family: localFont.name; pointSize: 10; bold: false;}
                                text: qsTr( errorinfo );
                                onTextChanged: {
                                    console.log("current text:" + text);
                                }
                            }
                            SequentialAnimation {
                                id: errorParallel;
                                running: false;
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 1000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 2000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 0; duration: 1000 }
                            }
                        }
                        //Item { width: 186; height: 23; }
                        /*-------------------------------------------
                         * 查询
                         ---------------------------------------------*/
                        PushButtons{
                             width: 86; height: 23;
                             label: qsTr("查询(S)");
                             onClicked:{ trackShowModel.dealQueryTrack( spinBoxState, hourEdit.text,
                                                                       getTime(startDateTime.getyear, startDateTime.getmonth, startDateTime.getday,
                                                                               startDateTime.gethour, startDateTime.getminute, startDateTime.getsecond),
                                                                       getTime(endDateTime.getyear, endDateTime.getmonth, endDateTime.getday,
                                                                               endDateTime.gethour, endDateTime.getminute, endDateTime.getsecond)); }
                        }
                        /*-------------------------------------------
                         * 取消
                         ---------------------------------------------*/
                        PushButtons{
                             width: 86; height: 23;
                             label: qsTr("取消(C)");
                             onClicked: trackShowModel.dealClose();
                        }
                    }
                }
            }

        }
    }
}
