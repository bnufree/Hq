import QtQuick 1.1
import "Components"

Rectangle {
    width: 100; height: 62; color: "#3e729f"
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int rotationvalue: 0;
    property int rotation: 0;

    property string m_siteName: "";                       //区站编号 00001 是南岭、00002是林诗岛
    property string m_lon: "";                            //经度
    property string m_lat: "";                            //纬度
    property string m_observation_hight: "";              //观测场拔海高度
    property string m_sensor_hight: "";                   //气压传感器拔海高度
    property string m_observation_way: "";                //观测方式
    property string m_time: "";                           //时间
    property string m_wind_10min_dir: "";                 //十分钟风向
    property string m_wind_10min_speed: "";               //5十分钟风速
    property string m_wind_max_dir: "";                   //6最大风速的风向
    property string m_wind_max_speed: "";                 //7最大风速
    property string m_wind_max_speed_time: "";            //8最大风速出现时间

    property string m_wind_instanta_dir: "";              //9瞬时风向   *
    property string m_wind_instanta_speed: "";            //10瞬时风速  *
    property string m_wind_extreme_dir: "";               //11极大风向
    property string m_wind_extreme_speed: "";             //12极大风速
    property string m_wind_extreme_speed_time: "";        //13极大风速出现时间

    property string m_precipitation: "";                  //14降水量  *
    property string m_airtemperature: "";                 //15气温   *
    property string m_airtemperature_max: "";             //16最高气温
    property string m_airtemperature_max_time: "";        //17最高气温出现时间
    property string m_airtemperature_min: "";             //18最低气温
    property string m_airtemperature_min_time: "";        //19最低气温出现时间

    property string m_relative_humidity: "";              //相对湿度   *
    property string m_relative_humidity_min: "";          //最小相对湿度
    property string m_relative_humidity_min_time: "";     //最小相对湿度出现时间

    property string m_air_pressure: "";                   //本站气压   *
    property string m_air_pressure_max: "";               //最高本站气压
    property string m_air_pressure_max_time: "";          //最高本站气压出现时间
    property string m_air_pressure_min: "";               //最低本站气压
    property string m_air_pressure_min_time: "";          //最高本站气压出现时间

    Column{
        anchors.fill: parent; anchors.margins: 2;
        Rectangle{ width: parent.width; height: 2; color: "#dedede"; }
        Rectangle {
            width: parent.width; height: parent.height-2;
            Column{
                anchors.fill: parent; anchors.margins: 10;
                Row{
                    /*-------------------------------------------
                     * 采集时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("采集时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_time; readOnly: true;
                            KeyNavigation.tab: siteNameText;
                        }
                    }

                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Item{ width: 750; height: 19; }
                        /*-------------------------------------------
                        * 错误提示信息
                        ---------------------------------------------*/
                        Rectangle {
                            id: errorButton; width: 750; height: 22;
                            color: "red"; radius: 2; clip: true; opacity: 0;
                            TextEdit {
                                id: errorinfoText;
                                anchors.centerIn: parent; text: "";
                                color: "#FAFAFA"; smooth: true;
                                font{ family: localFont.name; pointSize: 10; bold: true;}
                                selectByMouse: true; readOnly: true;
                            }
                            SequentialAnimation {
                                id: errorParallel;
                                running: false;
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 1000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 3000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 0; duration: 1000 }
                            }
                            Connections { target: MeteorologyDetailModel; onSendErrorTip: {
                                    errorParallel.running = true;
                                    errorinfoText.text = message;
                                }
                            }
                        }
                    }

                }
                Item{ width: parent.width; height: 4; }
                Rectangle{ width: parent.width; height: 2; color: Qt.rgba(0.3,0.3,0.3,0.1); }
                Item{ width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 站点名称
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("站点名称");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: siteNameText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_siteName; readOnly: true;
                            KeyNavigation.tab: m_lonText;
                        }
                    }
                    /*-------------------------------------------
                     * 经度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("经度");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_lonText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_lon; readOnly: true;
                            KeyNavigation.tab: m_latText;
                        }
                    }
                    /*-------------------------------------------
                     * 纬度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("纬度");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_latText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_lat; readOnly: true;
                            KeyNavigation.tab: m_observation_hightText;
                        }
                    }
                    /*-------------------------------------------
                     * 观测场拔海高度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("观测场拔海高度");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_observation_hightText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_observation_hight; readOnly: true;
                            KeyNavigation.tab: m_sensor_hightText;
                        }
                    }
                    /*-------------------------------------------
                     * 气压传感器拔海高度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("气压传感器拔海高度 (Pa)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_sensor_hightText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_sensor_hight; readOnly: true;
                            KeyNavigation.tab: m_observation_wayText;
                        }
                    }
                    /*-------------------------------------------
                     * 观测方式
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("观测方式");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_observation_wayText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_observation_way; readOnly: true;
                            KeyNavigation.tab: m_wind_10min_dirText;
                        }
                    }
                }

                Item{ width: parent.width; height: 4; }
                Rectangle{ width: parent.width; height: 2; color: Qt.rgba(0.3,0.3,0.3,0.1); }
                Item{ width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 十分钟风向
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("十分钟风向 (°)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_10min_dirText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_10min_dir; readOnly: true;
                            KeyNavigation.tab: m_wind_10min_speedText;
                        }
                    }
                    /*-------------------------------------------
                     * 十分钟风速
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("十分钟风速 (ｍ/ｓ)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_10min_speedText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_10min_speed; readOnly: true;
                            KeyNavigation.tab: m_wind_max_dirText;
                        }
                    }
                    /*-------------------------------------------
                     * 最大风速的风向
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最大风速的风向 (ｍ/ｓ)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_max_dirText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_max_dir; readOnly: true;
                            KeyNavigation.tab: m_wind_max_speedText;
                        }
                    }
                    /*-------------------------------------------
                     * 最大风速
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最大风速 (ｍ/ｓ)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_max_speedText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_max_speed; readOnly: true;
                            KeyNavigation.tab: m_wind_max_speed_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最大风速出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最大风速出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_max_speed_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_max_speed_time; readOnly: true;
                            KeyNavigation.tab: m_wind_instanta_dirText;
                        }
                    }
                    /*-------------------------------------------
                     * 瞬时风向
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("瞬时风向 (°)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_instanta_dirText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_instanta_dir; readOnly: true;
                            KeyNavigation.tab: m_wind_instanta_speedText;
                        }
                    }
                }
                Row{
                    /*-------------------------------------------
                     * 瞬时风速
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("瞬时风速 (ｍ/ｓ)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_instanta_speedText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_instanta_speed; readOnly: true;
                            KeyNavigation.tab: m_wind_extreme_dirText;
                        }
                    }
                    /*-------------------------------------------
                     * 极大风向
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("极大风向 (°)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_extreme_dirText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_extreme_dir; readOnly: true;
                            KeyNavigation.tab: m_wind_extreme_speedText;
                        }
                    }
                    /*-------------------------------------------
                     * 极大风速
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("极大风速 (ｍ/ｓ)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_extreme_speedText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_extreme_speed; readOnly: true;
                            KeyNavigation.tab: m_wind_extreme_speed_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 极大风速出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("极大风速出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_wind_extreme_speed_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_wind_extreme_speed_time; readOnly: true;
                            KeyNavigation.tab: m_precipitationText;
                        }
                    }
                    /*-------------------------------------------
                     * 降水量
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("降水量 (mm)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_precipitationText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_precipitation; readOnly: true;
                            KeyNavigation.tab: m_airtemperatureText;
                        }
                    }
                }
                Item{ width: parent.width; height: 4; }
                Rectangle{ width: parent.width; height: 2; color: Qt.rgba(0.3,0.3,0.3,0.1); }
                Item{ width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 气温
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("气温 (℃)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_airtemperatureText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_airtemperature; readOnly: true;
                            KeyNavigation.tab: m_airtemperature_maxText;
                        }
                    }
                    /*-------------------------------------------
                     * 最高气温
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最高气温 (℃)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_airtemperature_maxText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_airtemperature_max; readOnly: true;
                            KeyNavigation.tab: m_airtemperature_max_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最高气温出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最高气温出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_airtemperature_max_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_airtemperature_max_time; readOnly: true;
                            KeyNavigation.tab: m_airtemperature_minText;
                        }
                    }
                    /*-------------------------------------------
                     * 最低气温
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最低气温 (℃)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_airtemperature_minText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_airtemperature_min; readOnly: true;
                            KeyNavigation.tab: m_airtemperature_min_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最低气温出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最低气温出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_airtemperature_min_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_airtemperature_min_time; readOnly: true;
                            KeyNavigation.tab: m_relative_humidityText;
                        }
                    }
                }
                Item{ width: parent.width; height: 4; }
                Rectangle{ width: parent.width; height: 2; color: Qt.rgba(0.3,0.3,0.3,0.1); }
                Item{ width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 相对湿度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("相对湿度 (%)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_relative_humidityText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_relative_humidity; readOnly: true;
                            KeyNavigation.tab: m_relative_humidity_minText;
                        }
                    }
                    /*-------------------------------------------
                     * 最小相对湿度
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最小相对湿度 (%)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_relative_humidity_minText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_relative_humidity_min; readOnly: true;
                            KeyNavigation.tab: m_relative_humidity_min_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最小相对湿度出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最小相对湿度出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_relative_humidity_min_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_relative_humidity_min_time; readOnly: true;
                            KeyNavigation.tab: m_air_pressureText;
                        }
                    }
                }
                Item{ width: parent.width; height: 4; }
                Rectangle{ width: parent.width; height: 2; color: Qt.rgba(0.3,0.3,0.3,0.1); }
                Item{ width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 本站气压
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("本站气压 (Pa)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_air_pressureText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_air_pressure; readOnly: true;
                            KeyNavigation.tab: m_air_pressure_maxText;
                        }
                    }
                    /*-------------------------------------------
                     * 最高本站气压
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最高本站气压 (Pa)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_air_pressure_maxText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_air_pressure_max; readOnly: true;
                            KeyNavigation.tab: m_air_pressure_max_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最高本站气压出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最高本站气压出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_air_pressure_max_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_air_pressure_max_time; readOnly: true;
                            KeyNavigation.tab: m_air_pressure_minText;
                        }
                    }
                    /*-------------------------------------------
                     * 最低本站气压
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最低本站气压 (Pa)");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_air_pressure_minText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_air_pressure_min; readOnly: true;
                            KeyNavigation.tab: m_air_pressure_min_timeText;
                        }
                    }
                    /*-------------------------------------------
                     * 最低本站气压出现时间
                     ---------------------------------------------*/
                    Column{
                        width: 175; height: 51;  spacing: 3;
                        Text {
                            anchors.leftMargin: 4;
                            color: "#1a1a1a"; smooth: true; text: qsTr("最低本站气压出现时间");
                            font{ family: localFont.name; pointSize: 10;}
                        }
                        LineEdit {
                            id: m_air_pressure_min_timeText;
                            width: 161; height: 22; clip: true; color: "#333333";
                            text: m_air_pressure_min_time; readOnly: true;
                            KeyNavigation.tab: m_timeText;
                        }
                    }
                }
            }
        }
    }
    Connections{ target: MeteorologyDetailModel; onSendMeteorologyDetails:{
            m_siteName = siteName;                                      //区站编号 00001 是南岭、00002是林诗岛
            m_lon = lon;                                                //经度
            m_lat = lat;                                                //纬度
            m_observation_hight = observation_hight;                    //观测场拔海高度
            m_sensor_hight = sensor_hight;                              //气压传感器拔海高度
            m_observation_way = observation_way;                        //观测方式
            m_time = time;                                              //时间
            m_wind_10min_dir = wind_10min_dir;                          //十分钟风向
            m_wind_10min_speed = wind_10min_speed;                      //5十分钟风速
            m_wind_max_dir = wind_max_dir;                              //6最大风速的风向
            m_wind_max_speed = wind_max_speed;                          //7最大风速
            m_wind_max_speed_time = wind_max_speed_time;                //8最大风速出现时间

            m_wind_instanta_dir = wind_instanta_dir;                    //9瞬时风向   *
            m_wind_instanta_speed = wind_instanta_speed;                //10瞬时风速  *
            m_wind_extreme_dir = wind_extreme_dir;                      //11极大风向
            m_wind_extreme_speed = wind_extreme_speed;                  //12极大风速
            m_wind_extreme_speed_time = wind_extreme_speed_time;        //13极大风速出现时间

            m_precipitation = precipitation;                            //14降水量  *
            m_airtemperature = airtemperature;                          //15气温   *
            m_airtemperature_max = airtemperature_max;                  //16最高气温
            m_airtemperature_max_time = airtemperature_max_time;        //17最高气温出现时间
            m_airtemperature_min = airtemperature_min;                  //18最低气温
            m_airtemperature_min_time = airtemperature_min_time;        //19最低气温出现时间

            m_relative_humidity = relative_humidity;                    //相对湿度   *
            m_relative_humidity_min = relative_humidity_min;            //最小相对湿度
            m_relative_humidity_min_time = relative_humidity_min_time;  //最小相对湿度出现时间

            m_air_pressure = air_pressure;                              //本站气压   *
            m_air_pressure_max = air_pressure_max;                      //最高本站气压
            m_air_pressure_max_time = air_pressure_max_time;            //最高本站气压出现时间
            m_air_pressure_min = air_pressure_min;                      //最高本站气压
            m_air_pressure_min_time = air_pressure_min_time;            //最低本站气压出现时间
        }
    }
}
