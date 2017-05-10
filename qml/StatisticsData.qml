import QtQuick 1.1

Rectangle {
    width: 100; height: 62; color: Qt.rgba(1,1,1,0.4 );

    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }

    property string m_passenger_ship_total: "---";               //客船
    property string m_cargo_ship_total: "---";                   //货船
    property string m_tank_ship_total: "---";                    //油船
    property string m_danger_ship_total: "---";                  //危险船
    property string m_other_ship_total: "---";                   //其他类型船

    Column{
        anchors.fill: parent; anchors.leftMargin: 10; anchors.topMargin: 8; spacing: 6;
        /*-------------------------------------------
        * 客 船
        ---------------------------------------------*/
        Row{
            width: parent.width; height: 26; spacing: 6;
            Text {
                anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: qsTr("客船:");
            }
            TextEdit {
                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                selectByMouse: true; readOnly: true;
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: m_passenger_ship_total;
            }
        }
        /*-------------------------------------------
        * 货 船
        ---------------------------------------------*/
        Row{
            width: parent.width; height: 26; spacing: 6;
            Text {
                anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: qsTr("货船:");
            }
            TextEdit {
                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                selectByMouse: true; readOnly: true;
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: m_cargo_ship_total;
            }
        }
        /*-------------------------------------------
        * 油 船
        ---------------------------------------------*/
        Row{
            width: parent.width; height: 26; spacing: 6;
            Text {
                anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: qsTr("油船:");
            }
            TextEdit {
                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                selectByMouse: true; readOnly: true;
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: m_tank_ship_total;
            }
        }
        /*-------------------------------------------
        * 危险船
        ---------------------------------------------*/
        Row{
            width: parent.width; height: 26; spacing: 6;
            Text {
                anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: qsTr("危险船:");
            }
            TextEdit {
                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                selectByMouse: true; readOnly: true;
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: m_danger_ship_total;
            }
        }
        /*-------------------------------------------
        * 其他船
        ---------------------------------------------*/
        Row{
            width: parent.width; height: 26; spacing: 6;
            Text {
                anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: qsTr("其他船:");
            }
            TextEdit {
                width: 250; anchors.verticalCenter: parent.verticalCenter;  color: "#000000";
                selectByMouse: true; readOnly: true;
                font{ family: localFont.name; pointSize: 11; bold: false; }
                smooth: true; text: m_other_ship_total;
            }
        }
    }

    Connections {
        target: StatisticsDataModel;
        onSendShipStatistics: {
            m_passenger_ship_total = passenger_ship_total;              //客船
            m_cargo_ship_total = cargo_ship_total;                      //货船
            m_tank_ship_total = tank_ship_total;                        //油船
            m_danger_ship_total = danger_ship_total;                    //危险船
            m_other_ship_total = other_ship_total;                      //其他类型船
        }
    }
}
