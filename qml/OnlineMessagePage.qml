import QtQuick 1.1

Item {
    width: 136; height: 62;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property bool power_a : false;
    property bool power_b : false;
    property bool power_c : false;
    property variant alarmColor: ["transparent", "#ff0000","#ffcd00","#feff00","#0067cb","transparent"];
    property string m_sitecode_01: "---";                           //站点区号  00001 是南岭、00002是林诗岛
    property string m_time_01: "---";                               //更新时间 0334
    property string m_wind_instanta_dir_01: "0";                    //瞬时风向
    property string m_wind_instanta_speed_01: "0";                  //瞬时风速
    property int m_wind_instanta_speed_alarm_01: 0;                 //瞬时风速 是否报警
    property string m_precipitation_01: "0";                        //降水量  *
    property int m_precipitation_alarm_01: 0;                       //降水量  * 是否报警
    property string m_airtemperature_01: "0";                       //气温
    property int m_airtemperature_alarm_01: 0;                      //气温 是否报警
    property string m_relative_humidity_01: "0";                    //相对湿度
    property int m_relative_humidity_alarm_01: 0;                   //相对湿度 是否报警
    property string m_air_pressure_01: "0";                         //本站气压
    property int m_air_pressure_alarm_01: 0;                        //本站气压 是否报警

    property string m_sitecode_02: "---";                           //站点区号  00001 是南岭、00002是林诗岛
    property string m_time_02: "---";                               //更新时间 0334
    property string m_wind_instanta_dir_02: "0";                    //瞬时风向
    property string m_wind_instanta_speed_02: "0";                  //瞬时风速
    property int m_wind_instanta_speed_alarm_02: 0;                 //瞬时风速 是否报警
    property string m_precipitation_02: "0";                        //降水量  *
    property int m_precipitation_alarm_02: 0;                       //降水量  * 是否报警
    property string m_airtemperature_02: "0";                       //气温
    property int m_airtemperature_alarm_02: 0;                      //气温 是否报警
    property string m_relative_humidity_02: "0";                    //相对湿度
    property int m_relative_humidity_alarm_02: 0;                   //相对湿度 是否报警
    property string m_air_pressure_02: "0";                         //本站气压
    property int m_air_pressure_alarm_02: 0;                        //本站气压 是否报警

    Row{
        anchors.fill: parent;
        Rectangle{
            id: rectangel1;
            width: 408; height: parent.height; color: "#517da1"; opacity: 0.9; MouseArea{ anchors.fill: parent; }
            Column{
                anchors{ fill: parent; leftMargin: 3; rightMargin: 3; } spacing: 0;
                Rectangle {
                    width: parent.width; height: 23; color: "#3e3e3e"; clip: true;
                    Rectangle {
                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: Qt.rgba(1,1,1,0.77);
                        Rectangle {
                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#517da1";
                            /*-------------------------------------------
                             * A相内容
                             ---------------------------------------------*/
                            Row{
                                anchors.fill: parent; spacing: 2;
                                Rectangle {
                                    id: aButtons1;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        anchors.centerIn: parent;
                                        color: "#fffc00"; smooth: true; text: qsTr("A相");
                                        font{ family: localFont.name; pointSize: 11; bold: false; }
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: aButtons1; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: aButtons1; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }

                                Rectangle {
                                    id: aButtons2;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : a_v;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 KV");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: aButtons2; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: aButtons2; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }

                                Rectangle {
                                    id: aButtons3;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : a_a;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 A");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: aButtons3; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: aButtons3; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }



//                                Rectangle {
//                                    id: aButtons5;
//                                    width: (parent.width-10)/5; height: parent.height; color: "transparent";
//                                    Text {
//                                        anchors.verticalCenter: parent.verticalCenter;
//                                        color: "#ffffff"; smooth: true; text: qsTr("0.0 L/min");
//                                        font{ family: localFont.name; pointSize: 11;}
//                                    }
//                                    SequentialAnimation {
//                                        running: false; loops: Animation.Infinite;
//                                        ColorAnimation { target: aButtons5; property: "color"; to: "transparent"; duration: 1800; }
//                                        ColorAnimation { target: aButtons5; property: "color"; to: "#ff0000"; duration: 1800; }
//                                    }
//                                }
                            }
                        }
                    }
                }

                Rectangle {
                    width: parent.width; height: 23; color: "#3e3e3e"; clip: true;
                    Rectangle {
                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: Qt.rgba(1,1,1,0.77);
                        Rectangle {
                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#517da1";
                            /*-------------------------------------------
                             * B项内容
                             ---------------------------------------------*/
                            Row{
                                anchors.fill: parent; spacing: 2;
                                Rectangle {
                                    id: bButtons1;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        anchors.centerIn: parent;
                                        color: "#00bf02"; smooth: true; text: qsTr("B相");
                                        font{ family: localFont.name; pointSize: 11; bold: false; }
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: bButtons1; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: bButtons1; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }

                                Rectangle {
                                    id: bButtons2;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : b_v;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 KV");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: bButtons2; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: bButtons2; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }

                                Rectangle {
                                    id: bButtons3;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : b_a;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 A");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: bButtons3; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: bButtons3; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }



//                                Rectangle {
//                                    id: bButtons5;
//                                    width: (parent.width-10)/5; height: parent.height; color: "transparent";
//                                    Text {
//                                        anchors.verticalCenter: parent.verticalCenter;
//                                        color: "#ffffff"; smooth: true; text: qsTr("0.0 L/min");
//                                        font{ family: localFont.name; pointSize: 11;}
//                                    }
//                                    SequentialAnimation {
//                                        running: false; loops: Animation.Infinite;
//                                        ColorAnimation { target: bButtons5; property: "color"; to: "transparent"; duration: 1800; }
//                                        ColorAnimation { target: bButtons5; property: "color"; to: "#ff0000"; duration: 1800; }
//                                    }
//                                }
                            }
                        }
                    }
                }

                Rectangle {
                    width: parent.width; height: 23; color: "#3e3e3e"; clip: true;
                    Rectangle {
                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: Qt.rgba(1,1,1,0.77);
                        Rectangle {
                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#517da1";
                            /*-------------------------------------------
                             * C项内容
                             ---------------------------------------------*/
                            Row{
                                anchors.fill: parent; spacing: 2;
                                Rectangle {
                                    id: cButtons1;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        anchors.centerIn: parent;
                                        color: "#ff0000"; smooth: true; text: qsTr("C相");
                                        font{ family: localFont.name; pointSize: 11; bold: false; }
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: cButtons1; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: cButtons1; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }
                                Rectangle {
                                    id: cButtons2;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : c_v;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 KV");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: cButtons2; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: cButtons2; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }

                                Rectangle {
                                    id: cButtons3;
                                    width: (parent.width)/3; height: parent.height; color: "transparent";
                                    Text {
                                        id : c_a;
                                        anchors.verticalCenter: parent.verticalCenter;
                                        color: "#ffffff"; smooth: true; text: qsTr("0 A");
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: cButtons3; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: cButtons3; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }



//                                Rectangle {
//                                    id: cButtons5;
//                                    width: (parent.width-10)/5; height: parent.height; color: "transparent";
//                                    Text {
//                                        anchors.verticalCenter: parent.verticalCenter;
//                                        color: "#ffffff"; smooth: true; text: qsTr("0.0 L/min");
//                                        font{ family: localFont.name; pointSize: 11;}
//                                    }
//                                    SequentialAnimation {
//                                        running: false; loops: Animation.Infinite;
//                                        ColorAnimation { target: cButtons5; property: "color"; to: "transparent"; duration: 1800; }
//                                        ColorAnimation { target: cButtons5; property: "color"; to: "#ff0000"; duration: 1800; }
//                                    }
//                                }
                            }
                        }
                    }
                }
                Connections{
                    target : messageModel;
                    onSendPowerMsg:{
                          var json = JSON.parse(reStr);
                          if(json)
                          {
                                tec_ctr_activePower.text = json.activePower;
                                tec_ctr_reactivePower.text = json.reactivePower;
                                for(var i=0; i< json.ary.length; ++i)
                                {
                                    var obj = json.ary[i];
                                    if(obj.G == 1)
                                    {
                                        a_v.text = obj.V;
                                        a_a.text = obj.A;
                                    }
                                    else if(obj.G == 2)
                                    {
                                        b_v.text = obj.V;
                                        b_a.text = obj.A;
                                    }
                                    else if(obj.G == 3)
                                    {
                                        c_v.text = obj.V;
                                        c_a.text = obj.A;
                                    }
                                }
                          }
                    }
                }

            }
        }
        //zxl P/Q
        Rectangle{
            width: 70; height: parent.height; color: "#517da1"; opacity: 0.9; MouseArea{ anchors.fill: parent; }
            Column{
                anchors{ fill: parent; leftMargin: 3; rightMargin: 3; } spacing: 0;
                Item{
                    width:parent.width; height:23
                    Text {
                        anchors.centerIn: parent
                        color:"white"
                        font{ family: localFont.name; pointSize: 11;}
                        text: qsTr("P/Q")
                    }
                }
                Item{
                    width:parent.width; height:23
                    Text {
                        id:tec_ctr_activePower
                        color:"white"
                        anchors.centerIn: parent
                        font{ family: localFont.name; pointSize: 11;}
                        text: qsTr("0")
                    }
                }
                Item{
                    width:parent.width; height:23
                    Text {
                        id:tec_ctr_reactivePower
                        color:"white"
                        anchors.centerIn: parent
                        font{ family: localFont.name; pointSize: 11;}
                        text: qsTr("0")
                    }
                }

            }
        }
        Rectangle{
            width: parent.width-rectangel1.width; height: 48; color: "#517da1"; opacity: 0.9; MouseArea{ anchors.fill: parent; }
            Column{
                anchors.fill: parent; spacing: 0;
                /*-------------------------------------------
                 * 站点区号00001
                 ---------------------------------------------*/
                Rectangle {
                    width: 562; height: 23; color: "#3e3e3e"; clip: true;
                    Rectangle {
                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: Qt.rgba(1,1,1,0.77);
                        Rectangle {
                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#517da1";
                            Row{
                                anchors.fill: parent; spacing: 2;
                                /*-------------------------------------------
                                 * 时间
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons1;
                                    width: (parent.width-10)/8; height: parent.height; color: "transparent";
                                    TextEdit {
                                        id: timeText;
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_time_01;
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        id: timeError;
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: buttons1; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons1; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 站点区号
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons2;
                                    width: (parent.width-10)/8; height: parent.height; color: site1MouseArea.containsMouse ? "green" : "transparent" ;
                                    Text {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true; text: m_sitecode_01;
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    MouseArea{
                                        id: site1MouseArea;
                                        anchors.fill: parent; onClicked: {
                                            if( m_sitecode_01 != "---" )
                                            {
                                                messageModel.openMeteorologyDetail("00001");
                                            }
                                        }
                                    }
                                }
                                /*-------------------------------------------
                                 * 瞬时风向
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons3;
                                    width: (parent.width-10)/8; height: parent.height; color: "transparent";
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_wind_instanta_dir_01 + " °";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false; loops: Animation.Infinite;
                                        ColorAnimation { target: buttons3; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons3; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 瞬时风速
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons4;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_wind_instanta_speed_alarm_01];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_wind_instanta_speed_01 + " m/s";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: buttons4; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons4; property: "color"; to: alarmColor[m_wind_instanta_speed_alarm_01]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 降水量
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons5;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_precipitation_alarm_01];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_precipitation_01 + " ml";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: buttons5; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons5; property: "color"; to: alarmColor[m_precipitation_alarm_01]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 气温
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons6;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_airtemperature_alarm_01];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_airtemperature_01 + " ℃";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: buttons6; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons6; property: "color"; to: alarmColor[m_airtemperature_alarm_01]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 相对湿度
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons7;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_relative_humidity_alarm_01];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_relative_humidity_01 + " %";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: buttons7; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons7; property: "color"; to: alarmColor[m_relative_humidity_alarm_01]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 本站气压
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: buttons8;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_air_pressure_alarm_01];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_air_pressure_01 + " pa";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: buttons8; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: buttons8; property: "color"; to: alarmColor[m_air_pressure_alarm_01]; duration: 1800; }
                                    }
                                }

                            }
                        }
                        Connections {
                            target: messageModel;
                            onSendWeatherDataStruct001: {
                                m_sitecode_01 = sitecode;                                           //站点区号  00001 是南岭、00002是林诗岛
                                m_time_01 = time;                                                   //更新时间 0334
                                m_wind_instanta_dir_01 = wind_instanta_dir;                         //瞬时风向
                                m_wind_instanta_speed_01 = wind_instanta_speed;                     //瞬时风速
                                m_wind_instanta_speed_alarm_01 = wind_instanta_speed_alarm;         //瞬时风速 是否报警
                                m_precipitation_01 = precipitation;                                 //降水量  *
                                m_precipitation_alarm_01 = precipitation_alarm;                     //降水量  * 是否报警
                                m_airtemperature_01 = airtemperature;                               //气温
                                m_airtemperature_alarm_01 = airtemperature_alarm;                   //气温 是否报警
                                m_relative_humidity_01 = relative_humidity;                         //相对湿度
                                m_relative_humidity_alarm_01 = relative_humidity_alarm;             //相对湿度 是否报警
                                m_air_pressure_01 = air_pressure;                                   //本站气压
                                m_air_pressure_alarm_01 = air_pressure_alarm;                       //本站气压 是否报警
                            }
                        }
                    }
                }
                /*-------------------------------------------
                 * 站点区号00002
                 ---------------------------------------------*/
                Rectangle {
                    width: 562; height: 23; color: "#3e3e3e"; clip: true; anchors.bottomMargin: 1;
                    Rectangle {
                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: Qt.rgba(1,1,1,0.77);
                        Rectangle {
                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#517da1";

                            Row{
                                anchors.fill: parent; spacing: 2;
                                /*-------------------------------------------
                                 * 时间
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons1;
                                    width: (parent.width-10)/8; height: parent.height; color: "transparent";
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_time_02;
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        ColorAnimation { target: tButtons1; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons1; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 站点区号
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons2;
                                    width: (parent.width-10)/8; height: parent.height; color: site2MouseArea.containsMouse ? "green" : "transparent";
                                    Text {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true; text: m_sitecode_02;
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    MouseArea{
                                        id: site2MouseArea;
                                        anchors.fill: parent;
                                        onClicked: {
                                            if( m_sitecode_02 != "---" )
                                            {
                                                messageModel.openMeteorologyDetail("00002");
                                            }
                                        }
                                    }
                                }
                                /*-------------------------------------------
                                 * 瞬时风向
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons3;
                                    width: (parent.width-10)/8; height: parent.height; color: "transparent";
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_wind_instanta_dir_02 + " °";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        ColorAnimation { target: tButtons3; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons3; property: "color"; to: "#ff0000"; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 瞬时风速
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons4;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_wind_instanta_speed_alarm_02];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_wind_instanta_speed_02 + " m/s";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: tButtons4; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons4; property: "color"; to: alarmColor[m_wind_instanta_speed_alarm_02]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 降水量
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons5;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_precipitation_alarm_02];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_precipitation_02 + " ml";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: tButtons5; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons5; property: "color"; to: alarmColor[m_precipitation_alarm_02]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 气温
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons6;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_airtemperature_alarm_02];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_airtemperature_02 + " ℃";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false;
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: tButtons6; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons6; property: "color"; to: alarmColor[m_airtemperature_alarm_02]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 相对湿度
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons7;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_relative_humidity_alarm_02];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_relative_humidity_02 + " %";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: tButtons7; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons7; property: "color"; to: alarmColor[m_relative_humidity_alarm_02]; duration: 1800; }
                                    }
                                }
                                /*-------------------------------------------
                                 * 本站气压
                                 ---------------------------------------------*/
                                Rectangle {
                                    id: tButtons8;
                                    width: (parent.width-10)/8; height: parent.height; color: alarmColor[m_air_pressure_alarm_02];
                                    TextEdit {
                                        anchors.centerIn: parent;
                                        color: "#ffffff"; smooth: true;
                                        selectByMouse: true; readOnly: true; text: m_air_pressure_02 + " pa";
                                        font{ family: localFont.name; pointSize: 11;}
                                    }
                                    SequentialAnimation {
                                        running: false
                                        loops: Animation.Infinite;
                                        ColorAnimation { target: tButtons8; property: "color"; to: "transparent"; duration: 1800; }
                                        ColorAnimation { target: tButtons8; property: "color"; to: alarmColor[m_air_pressure_alarm_02]; duration: 1800; }
                                    }
                                }
                            }
                        }
                    }
                    Connections {
                        target: messageModel;
                        onSendWeatherDataStruct002: {
                            m_sitecode_02 = sitecode_02;                                          //站点区号  00001 是南岭、00002是林诗岛
                            m_time_02 = time_02;                                                  //更新时间 0334
                            m_wind_instanta_dir_02 = wind_instanta_dir_02;                        //瞬时风向
                            m_wind_instanta_speed_02 = wind_instanta_speed_02;                    //瞬时风速
                            m_wind_instanta_speed_alarm_02 = wind_instanta_speed_alarm_02;        //瞬时风速 是否报警
                            m_precipitation_02 = precipitation_02;                                //降水量  *
                            m_precipitation_alarm_02 = precipitation_alarm_02;                    //降水量  * 是否报警
                            m_airtemperature_02 = airtemperature_02;                              //气温
                            m_airtemperature_alarm_02 = airtemperature_alarm_02;                  //气温 是否报警
                            m_relative_humidity_02 = relative_humidity_02;                        //相对湿度
                            m_relative_humidity_alarm_02 = relative_humidity_alarm_02;            //相对湿度 是否报警
                            m_air_pressure_02 = air_pressure_02;                                  //本站气压
                            m_air_pressure_alarm_02 = air_pressure_alarm_02;                      //本站气压 是否报警
                        }
                    }
                }
            }
        }
    }
}
