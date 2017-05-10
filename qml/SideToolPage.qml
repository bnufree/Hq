/****************************************************************************
**
* @Project : Frameworks
* @File: sidebar.qml
* @Description: 侧边栏Toolbar
* @Copyright: Copyright (c) 2013
* @Company: 深圳置辰海信科技有限公司
* @WebSite: http://www.szcenterstar.com/
* @author: 框架本身
* @Revision History
* <pre>
* ----------------------------------------------------------------------
*   Ver     Date       Who        Comments
* ----- ----------  --------  ------------------------------------------
*   1.0  2013/08/12    叶国雄      FOR QT 4.8
* ----------------------------------------------------------------------
**
****************************************************************************/
import QtQuick 1.1

Item {
    id: isidebar; width: 42; height: 708;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    Column{
        spacing: 1;
        anchors.fill: parent;
        /*-------------------------------------------
         * 工具
         ---------------------------------------------*/
        Row {
            width: parent.width; height: 40;
            BorderImage{
                width: 42; height: parent.height;
                horizontalTileMode: BorderImage.Stretch;
                verticalTileMode: BorderImage.Stretch;
                border { left: 1; top: 1; right: 1; bottom: 1 }
                source: toolMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
                Column{
                    anchors.centerIn: parent;
                    spacing: 1;
                    BorderImage {
                        width: 18; height: 18;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        border { left: 2; top: 2; right: 2; bottom: 2 }
                        horizontalTileMode: BorderImage.Stretch;
                        verticalTileMode: BorderImage.Stretch;
                        source: "qrc:/core/qml/image/sidetool.png";
                        smooth: true;
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        color: "white"; text: qsTr("工具");
                        font{ family: localFont.name; pointSize: 9; bold: true }

                    }

                }

                BorderImage {
                    width: 6; height: 6;
                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
                    border { left: 2; top: 2; right: 2; bottom: 2 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/sideangle.png";
                    smooth: true;
                }
                MouseArea{
                    id : toolMouseArea;
                    anchors.fill: parent;
                    onClicked: sideToolModel.dealContextToolMenu( 72,96 );
                }
            }
        }
        /*-------------------------------------------
         * 绘制
         ---------------------------------------------*/
//        Row {
//            width: parent.width; height: 40;
//            BorderImage{
//                width: 42; height: parent.height;
//                horizontalTileMode: BorderImage.Stretch;
//                verticalTileMode: BorderImage.Stretch;
//                border { left: 1; top: 1; right: 1; bottom: 1 }
//                source: homeMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
//                Column{
//                    anchors.centerIn: parent;
//                    spacing: 1;
//                    BorderImage {
//                        width: 18; height: 18;
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        border { left: 2; top: 2; right: 2; bottom: 2 }
//                        horizontalTileMode: BorderImage.Stretch;
//                        verticalTileMode: BorderImage.Stretch;
//                        source: "qrc:/core/qml/image/sidedraw.png";
//                        smooth: true;
//                    }
//                    Text {
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        color: "white"; text: qsTr("绘制");
//                        font{ family: localFont.name; pointSize: 9; bold: true }
//                    }
//                }
//                BorderImage {
//                    width: 6; height: 6;
//                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
//                    border { left: 2; top: 2; right: 2; bottom: 2 }
//                    horizontalTileMode: BorderImage.Stretch;
//                    verticalTileMode: BorderImage.Stretch;
//                    source: "qrc:/core/qml/image/sideangle.png";
//                    smooth: true;
//                }
//                MouseArea{
//                    id : drawMouseArea;
//                    anchors.fill: parent;
//                    onClicked: sideToolModel.dealContextDrawMenu( 72,148 );
//                }
//            }
//        }
        /*-------------------------------------------
         * 操作
         ---------------------------------------------*/
        Row {
            width: parent.width; height: 40;
            BorderImage{
                width: 42; height: parent.height;
                horizontalTileMode: BorderImage.Stretch;
                verticalTileMode: BorderImage.Stretch;
                border { left: 1; top: 1; right: 1; bottom: 1 }
                source: operatMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
                Column{
                    anchors.centerIn: parent;
                    spacing: 1;
                    BorderImage {
                        width: 18; height: 18;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        border { left: 2; top: 2; right: 2; bottom: 2 }
                        horizontalTileMode: BorderImage.Stretch;
                        verticalTileMode: BorderImage.Stretch;
                        source: "qrc:/core/qml/image/sideoperat.png";
                        smooth: true;
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        color: "white"; text: qsTr("操作");
                        font{ family: localFont.name; pointSize: 9; bold: true }
                    }
                }
                BorderImage {
                    width: 6; height: 6;
                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
                    border { left: 2; top: 2; right: 2; bottom: 2 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/sideangle.png";
                    smooth: true;
                }
                MouseArea{
                    id : operatMouseArea;
                    anchors.fill: parent;
                    onClicked: sideToolModel.dealContextOperatMenu( 72,136 );
                }
            }
        }
        /*-------------------------------------------
         * 基本
         ---------------------------------------------*/
        Row {
            width: parent.width; height: 40;
            BorderImage{
                width: 42; height: parent.height;
                horizontalTileMode: BorderImage.Stretch;
                verticalTileMode: BorderImage.Stretch;
                border { left: 1; top: 1; right: 1; bottom: 1 }
                source: homeMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
                Column{
                    anchors.centerIn: parent;
                    spacing: 1;
                    BorderImage {
                        width: 18; height: 18;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        border { left: 2; top: 2; right: 2; bottom: 2 }
                        horizontalTileMode: BorderImage.Stretch;
                        verticalTileMode: BorderImage.Stretch;
                        source: "qrc:/core/qml/image/sidehome.png";
                        smooth: true;
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        color: "white"; text: qsTr("基本");
                        font{ family: localFont.name; pointSize: 9; bold: true }
                    }
                }
                BorderImage {
                    width: 6; height: 6;
                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
                    border { left: 2; top: 2; right: 2; bottom: 2 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/sideangle.png";
                    smooth: true;
                }
                MouseArea{
                    id : homeMouseArea;
                    anchors.fill: parent;
                    onClicked: sideToolModel.dealContextBasicMenu( 72,190 );
                }
            }
        }
        /*-------------------------------------------
         * 颜色
         ---------------------------------------------*/
        Row {
            width: parent.width; height: 40;
            BorderImage{
                width: 42; height: parent.height;
                horizontalTileMode: BorderImage.Stretch;
                verticalTileMode: BorderImage.Stretch;
                border { left: 1; top: 1; right: 1; bottom: 1 }
                source: colorMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
                Column{
                    anchors.centerIn: parent;
                    spacing: 1;
                    BorderImage {
                        width: 18; height: 18;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        border { left: 2; top: 2; right: 2; bottom: 2 }
                        horizontalTileMode: BorderImage.Stretch;
                        verticalTileMode: BorderImage.Stretch;
                        source: "qrc:/core/qml/image/sidecolor.png";
                        smooth: true;
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        color: "white"; text: qsTr("颜色");
                        font{ family: localFont.name; pointSize: 9; bold: true }
                    }
                }
                BorderImage {
                    width: 6; height: 6;
                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
                    border { left: 2; top: 2; right: 2; bottom: 2 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/sideangle.png";
                    smooth: true;
                }
                MouseArea{
                    id : colorMouseArea;
                    anchors.fill: parent;
                    onClicked: sideToolModel.dealContextColorMenu( 72,178 );
                }
            }
        }
        /*-------------------------------------------
         * 图层
         ---------------------------------------------*/
        Row {
            width: parent.width; height: 40;
            BorderImage{
                width: 42; height: parent.height;
                horizontalTileMode: BorderImage.Stretch;
                verticalTileMode: BorderImage.Stretch;
                border { left: 1; top: 1; right: 1; bottom: 1 }
                source: layerMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
                Column{
                    anchors.centerIn: parent;
                    spacing: 1;
                    BorderImage {
                        width: 18; height: 18;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        border { left: 2; top: 2; right: 2; bottom: 2 }
                        horizontalTileMode: BorderImage.Stretch;
                        verticalTileMode: BorderImage.Stretch;
                        source: "qrc:/core/qml/image/sidelayer.png";
                        smooth: true;
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        color: "white"; text: qsTr("图层");
                        font{ family: localFont.name; pointSize: 9; bold: true }
                    }
                }
                BorderImage {
                    width: 6; height: 6;
                    anchors{ right: parent.right;rightMargin: 0; bottom: parent.bottom; bottomMargin: 0; }
                    border { left: 2; top: 2; right: 2; bottom: 2 }
                    horizontalTileMode: BorderImage.Stretch;
                    verticalTileMode: BorderImage.Stretch;
                    source: "qrc:/core/qml/image/sideangle.png";
                    smooth: true;
                }
                MouseArea{
                    id : layerMouseArea;
                    anchors.fill: parent;
                    onClicked: sideToolModel.dealContextLayerMenu( 72,218 );
                }
            }
        }

        /*-------------------------------------------
         * 工具
         ---------------------------------------------*/
//        Row {
//            width: parent.width; height: 40;
//            BorderImage{
//                width: 42; height: parent.height;
//                horizontalTileMode: BorderImage.Stretch;
//                verticalTileMode: BorderImage.Stretch;
//                border { left: 1; top: 1; right: 1; bottom: 1 }
//                source: videoMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
//                Column{
//                    anchors.centerIn: parent;
//                    spacing: 1;
//                    BorderImage {
//                        width: 18; height: 18;
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        border { left: 2; top: 2; right: 2; bottom: 2 }
//                        horizontalTileMode: BorderImage.Stretch;
//                        verticalTileMode: BorderImage.Stretch;
//                        source: "qrc:/core/qml/image/video.png";
//                        smooth: true;
//                    }
//                    Text {
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        color: "white"; text: qsTr("视频");
//                        font{ family: localFont.name; pointSize: 9; bold: true }
//                    }
//                }
//                MouseArea{
//                    id : videoMouseArea;
//                    anchors.fill: parent;
//                    onClicked: sideToolModel.dealOpenVideo();
//                }
//            }
//        }
//        /*-------------------------------------------
//         * 雷达
//         ---------------------------------------------*/
//        Row {
//            width: parent.width; height: 40;
//            BorderImage{
//                width: 42; height: parent.height;
//                horizontalTileMode: BorderImage.Stretch;
//                verticalTileMode: BorderImage.Stretch;
//                border { left: 1; top: 1; right: 1; bottom: 1 }
//                source: radarMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
//                Column{
//                    anchors.centerIn: parent;
//                    spacing: 1;
//                    BorderImage {
//                        width: 18; height: 18;
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        border { left: 2; top: 2; right: 2; bottom: 2 }
//                        horizontalTileMode: BorderImage.Stretch;
//                        verticalTileMode: BorderImage.Stretch;
//                        source: "qrc:/core/qml/image/video.png";
//                        smooth: true;
//                    }
//                    Text {
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        color: "white"; text: qsTr("雷达");
//                        font{ family: localFont.name; pointSize: 9; bold: true }
//                    }
//                }
//                MouseArea{
//                    id : radarMouseArea;
//                    anchors.fill: parent;
//                    onClicked: sideToolModel.dealRadarDevice();
//                }
//            }
//        }
//        /*-------------------------------------------
//         * 机层设备状态
//         ---------------------------------------------*/
//        Row {
//            width: parent.width; height: 40;
//            BorderImage{
//                width: 42; height: parent.height;
//                horizontalTileMode: BorderImage.Stretch;
//                verticalTileMode: BorderImage.Stretch;
//                border { left: 1; top: 1; right: 1; bottom: 1 }
//                source: machineMouseArea.containsMouse ? "qrc:/core/qml/image/toolbarbg_off.png" : "qrc:/core/qml/image/toolbarbg.png";
//                Column{
//                    anchors.centerIn: parent;
//                    spacing: 1;
//                    BorderImage {
//                        width: 18; height: 18;
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        border { left: 2; top: 2; right: 2; bottom: 2 }
//                        horizontalTileMode: BorderImage.Stretch;
//                        verticalTileMode: BorderImage.Stretch;
//                        source: "qrc:/core/qml/image/video.png";
//                        smooth: true;
//                    }
//                    Text {
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        color: "white"; text: qsTr("机层");
//                        font{ family: localFont.name; pointSize: 9; bold: true }
//                    }
//                }
//                MouseArea{
//                    id : machineMouseArea;
//                    anchors.fill: parent;
//                    onClicked: sideToolModel.dealMachineDevice();
//                }
//            }
//        }
    }
}
