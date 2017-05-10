import QtQuick 1.1

Item{
    width: 1600; height: 35;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property string scaleValue: "5000";
    property string longitudes: "---";
    property string latitudes: "---";
    property string coordinates: "---";

    property string m_passenger_ship_total: "---";               //客船
    property string m_cargo_ship_total: "---";                   //货船
    property string m_tank_ship_total: "---";                    //油船
    property string m_danger_ship_total: "---";                  //危险船
    property string m_other_ship_total: "---";                   //其他类型船

    MouseArea{ anchors.fill: parent; }
    Rectangle{
        width: parent.width; height: 33; color: Qt.rgba( 1,1,1,0.071 );
        anchors.verticalCenter: parent.verticalCenter;
        Row{
            anchors{ fill: parent; left: parent.left; leftMargin: 0; }
            Item{ width: 6; height: parent.height; }
            /*-------------------------------------------
             * 坐标
             ---------------------------------------------*/
            Column {
                width: 190; height: 27; spacing: 0; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Item{
                    width: parent.width; height: 18;
                    Text {
                        anchors.centerIn: parent;
                        color: "#000000"; text: coordinates;
                        font{ family: localFont.name; pointSize: 10; bold: true }
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "blue"; }
            }

            Item{ width: 30; height: parent.height; }
            /*-------------------------------------------
            * 客船
            ---------------------------------------------*/
            Column {
                width: 80; height: 27; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Row{
                    spacing: 6; anchors.horizontalCenter: parent.horizontalCenter; height: 18;
                    Text {
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        color: "#000000"; smooth: true; text: qsTr("客船:");
                    }
                    TextEdit {
                        color: "#000000";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        smooth: true; text: m_passenger_ship_total;
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "#0003ff"; }
            }
            /*-------------------------------------------
            * 货船
            ---------------------------------------------*/
            Item{ width: 12; height: parent.height; }
            Column {
                width: 80; height: 27; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Row{
                    spacing: 6; anchors.horizontalCenter: parent.horizontalCenter; height: 18;
                    Text {
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        color: "#000000"; smooth: true; text: qsTr("货船:");
                    }
                    TextEdit {
                        color: "#000000";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        smooth: true; text: m_cargo_ship_total;
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "#0003ff"; }
            }
            /*-------------------------------------------
            * 油船
            ---------------------------------------------*/
            Item{ width: 12; height: parent.height; }
            Column {
                width: 80; height: 27; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Row{
                    spacing: 6; anchors.horizontalCenter: parent.horizontalCenter; height: 18;
                    Text {
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        color: "#000000"; smooth: true; text: qsTr("油船:");
                    }
                    TextEdit {
                        color: "#000000";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        smooth: true; text: m_tank_ship_total;
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "#0003ff"; }
            }
            /*-------------------------------------------
            * 危险船
            ---------------------------------------------*/
            Item{ width: 12; height: parent.height; }
            Column {
                width: 80; height: 27; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Row{
                    spacing: 6; anchors.horizontalCenter: parent.horizontalCenter; height: 18;
                    Text {
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        color: "#000000"; smooth: true; text: qsTr("危险船:");
                    }
                    TextEdit {
                        color: "#000000";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        smooth: true; text: m_danger_ship_total;
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "#0003ff"; }
            }
            /*-------------------------------------------
            * 其他船
            ---------------------------------------------*/
            Item{ width: 12; height: parent.height; }
            Column {
                width: 80; height: 27; anchors.verticalCenter: parent.verticalCenter; clip: true;
                Row{
                    spacing: 6; anchors.horizontalCenter: parent.horizontalCenter; height: 18;
                    Text {
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        color: "#000000"; smooth: true; text: qsTr("其他船:");
                    }
                    TextEdit {
                        color: "#000000";
                        selectByMouse: true; readOnly: true;
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                        smooth: true; text: m_other_ship_total;
                    }
                }
                Rectangle{ width: parent.width; height: 2; color: "#0003ff"; }
            }

            Connections {
                target: toolTipModel;
                onSendShipStatistics: {
                    m_passenger_ship_total = passenger_ship_total;              //客船
                    m_cargo_ship_total = cargo_ship_total;                      //货船
                    m_tank_ship_total = tank_ship_total;                        //油船
                    m_danger_ship_total = danger_ship_total;                    //危险船
                    m_other_ship_total = other_ship_total;                      //其他类型船
                }
            }
        }
    }

    Connections { target: toolTipModel; onSendScale: { scaleValue = scales; longitudes = lons; latitudes = lats; coordinates = coordinate; } }
}
