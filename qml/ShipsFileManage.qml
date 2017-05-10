import QtQuick 1.1
import "Components"

Item {
    width: 100; height: 62;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property string vesselimagesource: ""
    property int i: 0;
    Rectangle{
        anchors.fill: parent; color: "#4177a5";
        Rectangle{
            anchors.fill: parent; radius: 2; anchors.margins:3; border{ width: 1; color: "#E5E5E5" }
            Column{
                anchors.fill: parent;  spacing: 0; anchors.margins: 8; clip: true;
                Rectangle{
                    width: 80; height: 24; color: "#4177a5";
                    Text {
                        anchors.centerIn: parent; smooth: true;
                        color: "#ffffff"; text: qsTr("船舶概述");
                        font{ family: localFont.name; pointSize: 10;}
                    }
                }
                Rectangle{ width: parent.width; height: 1; color: "#4177a5"; }
                Item { width: parent.width; height: 4; }
                Row{
                    /*-------------------------------------------
                     * 船舶图片
                     ---------------------------------------------*/
                    Item{

                        width: 298; height: 234;
                        BorderImage {
                            id: vesselimageImg;
                            anchors.fill: parent; anchors.margins: 6;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            border { left: 2; top: 2; right: 2; bottom: 2 }
                            horizontalTileMode: BorderImage.Stretch;
                            verticalTileMode: BorderImage.Stretch;
                            source: (vesselimagesource == "") ?  "qrc:/ecdisviewer/qml/images/ship.png" : vesselimagesource;
                            smooth: true;
                        }
                    }
                    Item { width: 3; height: parent.height; }
                    Column{
                        Row{
                            z:600;
                            /*-------------------------------------------
                             * 英文船名
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*船舶名称");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: vesselNameEnText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 20; KeyNavigation.tab: imoText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 船舶类型
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*船舶类型");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                ComboBox {
                                    id: vesseltypeText;
                                    color: "#ffffff";
                                    border.color: "#616161";
                                     width:161;height: 22;
                                    ListModel {
                                        id: comboBox1Model
                                        ListElement { idx: "0"; name: "其他船舶"; }
                                        ListElement { idx: "1"; name: "渔船"; }
                                        ListElement { idx: "2"; name: "客船"; }
                                        ListElement { idx: "3"; name: "货船"; }
                                        ListElement { idx: "4"; name: "油轮"; }
                                    }
                                    model: comboBox1Model;
                                    text: comboBox1Model.get(0).name;
                                }
                            }
                            /*-------------------------------------------
                             * IMO
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*IMO");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: imoText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 20; KeyNavigation.tab: mmsiText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * MMSI
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*MMSI");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: mmsiText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                validator: IntValidator{ bottom: 0; top: 1000000000; } KeyNavigation.tab: callsignText;
                                                font{ family: localFont.name; pointSize: 10;} readOnly: false;
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 呼号
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*呼号");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: callsignText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 20; KeyNavigation.tab: anchorweightText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }

                                        }
                                    }
                                }
                            }
                        }


                        Row{
                            /*-------------------------------------------
                             * 锚重
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*锚重");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: anchorweightText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                validator: DoubleValidator{ bottom: 0; top: 999; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: mainEngineNumberText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 主机数量
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("主机数量");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: mainEngineNumberText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: mainEnginePowerText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 主机功率
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("主机功率");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: mainEnginePowerText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: mainEngineKindText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 主机种类
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("主机种类");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: mainEngineKindText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 20; KeyNavigation.tab: dataSourceText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 数据来源
                             --------------------------F-------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*数据来源");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: dataSourceText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 10; KeyNavigation.tab: nationText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Row{
                            /*-------------------------------------------
                             * 国籍
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*国籍");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: nationText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 15; KeyNavigation.tab: maxSpeedText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 最大航速
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("*最大航速");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: maxSpeedText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; } KeyNavigation.tab: madeDateText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 建造日期
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("建造日期");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: madeDateText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 10; KeyNavigation.tab: updateLatestDateText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 最后更新日期
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("最后更新日期");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: updateLatestDateText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 20; KeyNavigation.tab: stateText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                                readOnly: true;
                                            }
                                        }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 当前状态
                             ---------------------------------------------*/
                            Column{
                                width: 175; height: 51;  spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("当前状态");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 161; height: 22; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: stateText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 3; KeyNavigation.tab: remarkText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Row{
                            /*-------------------------------------------
                             * 备注
                             ---------------------------------------------*/
                            Column{
                                width: 862; height: 75; spacing: 3;
                                Text {
                                    anchors.leftMargin: 4;
                                    color: "#1a1a1a"; smooth: true; text: qsTr("备注");
                                    font{ family: localFont.name; pointSize: 10;}
                                }
                                Rectangle {
                                    width: 862; height: 53; clip: true; color: "#616161";
                                    Rectangle {
                                        anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                        Rectangle {
                                            anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                            TextInput {
                                                id: remarkText;
                                                anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                                color: "#1a1a1a"; smooth: true; text: qsTr("");
                                                maximumLength: 30; KeyNavigation.tab: lengthText;
                                                font{ family: localFont.name; pointSize: 10;}
                                                onFocusChanged: if(focus) selectAll();
                                            }
                                        }
                                    }
                                }
                            }

                        }
                    }
                }
                Rectangle{
                    width: 80; height: 24; color: "#4177a5";
                    Text {
                        anchors.centerIn: parent; smooth: true;
                        color: "#ffffff"; text: qsTr("船舶尺寸");
                        font{ family: localFont.name; pointSize: 10;}
                    }
                }
                Rectangle{ width: parent.width; height: 1; color: "#4177a5"; }
                Item { width: parent.width; height: 4; }
                Column{
                    Row{
                        /*-------------------------------------------
                         * 船长
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("*船长");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: lengthText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: heightText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船高
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("*船高");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: heightText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: widthText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船宽
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("*船宽");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: widthText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: maxDraughtText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 最大吃水
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("最大吃水");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: maxDraughtText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: widthMoudledText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 型宽
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("型宽");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: widthMoudledText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: gwtText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 总吨
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("总吨");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: gwtText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: nwtText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 净吨
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("净吨");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: nwtText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: dwtText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }

                    }
                    Row{
                        /*-------------------------------------------
                         * 载重吨
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("载重吨");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: dwtText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: summerMeanDraftText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 夏季满载吃水
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("夏季满载吃水");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: summerMeanDraftText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: depthMoudledText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 型深
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("型深");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: depthMoudledText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: DoubleValidator{ bottom: 0; top: 9999.99; decimals: 2; notation:DoubleValidator.StandardNotation; }  KeyNavigation.tab: passengerQuantityMaxText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 最大载客人数
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("最大载客人数");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: passengerQuantityMaxText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: vehicleQuantityMaxText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 最大载车人数
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("最大载车人数");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vehicleQuantityMaxText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } KeyNavigation.tab: portNationText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: 80; height: 24; color: "#4177a5";
                    Text {
                        anchors.centerIn: parent; smooth: true;
                        color: "#ffffff"; text: qsTr("船舶注册");
                        font{ family: localFont.name; pointSize: 10;}
                    }
                }
                Rectangle{ width: parent.width; height: 1; color: "#4177a5"; }
                Item { width: parent.width; height: 4; }
                Column{
                    Row{
                        /*-------------------------------------------
                         * 船籍港
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船籍港");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: portNationText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 20; KeyNavigation.tab: vesselKindText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶种类
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶种类");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselKindText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 10; KeyNavigation.tab: vesselOwnerCnText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶所有人
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶所有人");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOwnerCnText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: vesselOwnerEnText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }

                        /*-------------------------------------------
                         * 船舶所有人英文名
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶所有人英文名");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOwnerEnText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: vesselOwnerPhoneText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶所有人电话
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶所有人电话");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOwnerPhoneText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 11; KeyNavigation.tab: vesselOwnerAddressText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            validator: RegExpValidator{regExp: /^[0-9]{1,11}$/}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶所有人地址
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶所有人地址");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOwnerAddressText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: operatornameText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶经营人名称
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶经营人名称");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: operatornameText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 20; KeyNavigation.tab: operatorphoneText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }

                    }
                    Row{
                        /*-------------------------------------------
                         * 船舶经营人电话
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶经营人电话");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: operatorphoneText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 11; KeyNavigation.tab: operatoraddressText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            validator: RegExpValidator{regExp: /^[0-9]{1,11}$/}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶经营人地址
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶经营人地址");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: operatoraddressText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 20; KeyNavigation.tab: administratornameText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶管理人名称
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶管理人名称");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: administratornameText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 20; KeyNavigation.tab: administratorphoneText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶管理人电话
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶管理人电话");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: administratorphoneText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 11; KeyNavigation.tab: administratoraddressText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            validator: RegExpValidator{regExp: /^[0-9]{1,11}$/}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶管理人地址
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶管理人地址");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: administratoraddressText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: vesselOperatorFidText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 经营人FID
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("经营人FID");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOperatorFidText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: vesselOwnerFidText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 所有人FID
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("所有人FID");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselOwnerFidText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 30; KeyNavigation.tab: vesselnumberText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Row{
                        /*-------------------------------------------
                         * 船舶代号
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶代号");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselnumberText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 10; KeyNavigation.tab: vesselRegisterIdText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 登记号码
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("登记号码");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselRegisterIdText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 14; KeyNavigation.tab: initialRegisterNumberText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 初次登记号码
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("初次登记号码");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: initialRegisterNumberText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 14; KeyNavigation.tab: securityLevelText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 安全等级
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("安全等级");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: securityLevelText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 9; KeyNavigation.tab: vesselAreaText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 船舶区域分类
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("船舶区域分类");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselAreaText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            maximumLength: 10; KeyNavigation.tab: vesselNameEnText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 编号
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3; visible: false;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("编号");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: aisIdxText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr(""); focus: true;
                                            font{ family: localFont.name; pointSize: 10;}
                                            validator: IntValidator{ bottom: 0; top: 1000000000; } //KeyNavigation.tab: vesselNameCnText;
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 来港次数
                         ---------------------------------------------*/
                        Column{
                            width: 168; height: 51;  spacing: 3; visible: false;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("来港次数");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: enterTimesText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr("");
                                            validator: IntValidator{ bottom: 0; top: 32767; } //KeyNavigation.tab: vesselAreaText;
                                            font{ family: localFont.name; pointSize: 10;}
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
                                }
                            }
                        }
                        /*-------------------------------------------
                         * 中文船名
                         ---------------------------------------------*/
                        Column{
                            width: 152; height: 51; spacing: 3; visible: false;
                            Text {
                                anchors.leftMargin: 4;
                                color: "#1a1a1a"; smooth: true; text: qsTr("中文船名");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            Rectangle {
                                width: 154; height: 22; clip: true; color: "#616161";
                                Rectangle {
                                    anchors { fill: parent; left: parent.left; top: parent.top; leftMargin: 1; topMargin: 1; } color: "#c2c2c2";
                                    Rectangle {
                                        anchors { fill: parent; right: parent.right; bottom: parent.bottom; rightMargin: 1; bottomMargin: 1; } clip: true; color: "#ffffff";
                                        TextInput {
                                            id: vesselNameCnText;
                                            anchors{ fill: parent; left: parent.left; leftMargin: 2; } selectByMouse: true;
                                            color: "#1a1a1a"; smooth: true; text: qsTr(""); focus: true;
                                            font{ family: localFont.name; pointSize: 10;}
                                            maximumLength: 30; //KeyNavigation.tab: vesselNameEnText;
                                            onFocusChanged: if(focus) selectAll();
                                        }
                                    }
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
                            id: errorButton; width: 460; height: 26; color: "red"; radius: 2; clip: true; opacity: 0;
                            Text {
                                id: errorinfoText;
                                anchors.centerIn: parent;
                                color: "#FAFAFA"; smooth: true;
                                font{ family: localFont.name; pointSize: 10; bold: true;}
                            }
                            SequentialAnimation {
                                id: errorParallel;
                                running: false;
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 1000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 1; duration: 2000 }
                                NumberAnimation { target: errorButton; property: "opacity"; to: 0; duration: 1000 }
                            }
                        }
                        Item { width: 186; height: 23; }
                        /*-------------------------------------------
                         * 保存
                         ---------------------------------------------*/
                        Rectangle {
                            width: 86; height: 23; color: saveMouseArea.containsMouse ? "#ff8200" : "#508cc0";
                            Text {
                                anchors.centerIn: parent;
                                color: saveMouseArea.containsMouse ? "#ffffff" : "#1a1a1a";
                                smooth: true; text: qsTr("保存(S)");
                                font{ family: localFont.name; pointSize: 10;}
                            }

                            MouseArea{
                                id: saveMouseArea; anchors.fill: parent; hoverEnabled: true;

                                onClicked: {
                                    shipFileModel.dealSaveVesselInfo(
                                                aisIdxText.text,
                                                vesselNameCnText.text,                           //中文船名  长度 30
                                                vesselNameEnText.text,                           //英文船名  长度 20
                                                callsignText.text,                               //呼号
                                                nationText.text,                                 //国籍             长度 15
                                                vesselnumberText.text,                           //船舶代号  长度 10
                                              //  vesseltypeText.items[vesseltypeText.indexs],
                                                comboBox1Model.get(vesseltypeText.indexs).name,
                                              //  vesseltypeText.text,                             //船舶类型
                                                passengerQuantityMaxText.text,                   //最大载客人数
                                                vehicleQuantityMaxText.text,                     //最大载车人数
                                                lengthText.text,                                 //船长
                                                heightText.text,                                 //船高
                                                widthText.text,                                  //船宽
                                                maxSpeedText.text,                               //船速
                                                gwtText.text,                                    //总吨
                                                nwtText.text,                                    //净吨
                                                anchorweightText.text,                           //锚重
                                                mainEngineNumberText.text,                       //主机数量
                                                mainEnginePowerText.text,                        //主机功率
                                                mainEngineKindText.text,                         //主机种类 长度 20
                                                dwtText.text,                                    //载重吨
                                                stateText.text,                                  //当前状态  长度 3
                                                mmsiText.text,                                   //MMSI
                                                maxDraughtText.text,                             //最大吃水
                                                vesselRegisterIdText.text,                       //登记号码  长度 14
                                                initialRegisterNumberText.text,                  //初次登记号码   长度 14
                                                imoText.text,                                    //imo          长度 20
                                                madeDateText.text,                               //建造日期 yyyy-MM-dd         长度 10
                                                securityLevelText.text,                          //安全等级               长度 9
                                                widthMoudledText.text,                           //型宽
                                                depthMoudledText.text,                           //型深
                                                summerMeanDraftText.text,                        //夏季满载吃水
                                                enterTimesText.text,                             //来港次数
                                                vesselAreaText.text,                             //船舶区域分类               长度 10
                                                portNationText.text,                             //船籍港                     长度 30
                                                vesselKindText.text,                             //船舶种类：0：河船          长度 10
                                                dataSourceText.text,                             //数据来源：0                长度 10
                                                remarkText.text,                                 //备注                           长度 50
                                                updateLatestDateText.text,                       //最后更新日期 yyyy-MM-dd HH:mm:ss
                                                vesselOperatorFidText.text,                      //经营人FID           长度 50
                                                vesselOwnerFidText.text,                         //所有人FID              长度 50
                                                vesselOwnerCnText.text,                          //船舶所有人              长度 50
                                                vesselOwnerEnText.text,                          //船舶所有人英文名        长度 50
                                                vesselOwnerAddressText.text,                     //船舶所有人地址     长度 50
                                                operatorphoneText.text,                          //船舶经营人电话          长度 20
                                                operatoraddressText.text,                        //船舶经营人地址        长度 30
                                                operatornameText.text,                           //船舶经营人名称           长度 20
                                                administratornameText.text,                      //船舶管理人名称      长度 20
                                                administratorphoneText.text,                     //船舶管理人电话     长度 20
                                                administratoraddressText.text,                   //船舶管理人地址    长度 30
                                                vesselOwnerPhoneText.text,                       //船舶所有人电话   长度 11
                                                "vesselimageText.text"                           //船舶图片
                                                );
                                }
                            }

                        }
                        /*-------------------------------------------
                         * 取消
                         ---------------------------------------------*/
                        Rectangle{
                            width: 86; height: 23; color: cancelMouseArea.containsMouse ? "#ff8200" : "#508cc0";
                            Text {
                                anchors.centerIn: parent;
                                color: cancelMouseArea.containsMouse ? "#ffffff" : "#1a1a1a"; smooth: true; text: qsTr("关闭");
                                font{ family: localFont.name; pointSize: 10;}
                            }
                            MouseArea{ id: cancelMouseArea; anchors.fill: parent; hoverEnabled: true; onClicked: shipFileModel.dealClose(); }
                        }
                    }
                }
            }
        }
    }

    Connections { target: shipFileModel;onSendAlarmInfo: { errorParallel.running = isalarm; errorinfoText.text = errortext; } }

    Connections { target: shipFileModel;
        onSendVesselInfo: {
            aisIdxText.text = idx;                                      //编号
            vesselNameCnText.text =  vesselNameCn;                      //中文船名  长度 30
            vesselNameEnText.text =  vesselNameEn;                      //英文船名  长度 20
            callsignText.text = callsign;                               //呼号
            nationText.text =  nation;                                  //国籍             长度 15
            vesselnumberText.text =  vesselnumber;                      //船舶代号  长度 10
            vesseltypeText.indexs=0;
            for(i=0;i<5;i++)
            {
              if(vesseltype== comboBox1Model.get(i).name)
              vesseltypeText.text=comboBox1Model.get(i).name;
            }
                         //船舶类型
            passengerQuantityMaxText.text = passengerQuantityMax;       //最大载客人数
            vehicleQuantityMaxText.text =  vehicleQuantityMax;          //最大载车人数
            lengthText.text =  length;                                  //船长
            heightText.text =  height;                                  //船高
            widthText.text =  width;                                    //船宽
            maxSpeedText.text = maxSpeed;                               //船速
            gwtText.text = gwt;                                         //总吨
            nwtText.text = nwt;                                         //净吨
            anchorweightText.text = anchorweight;                       //锚重
            mainEngineNumberText.text = mainEngineNumber;               //主机数量
            mainEnginePowerText.text = mainEnginePower;                 //主机功率
            mainEngineKindText.text = mainEngineKind;                   //主机种类 长度 20
            dwtText.text = dwt;                                         //载重吨
            stateText.text =  state;                                    //当前状态  长度 3
            mmsiText.text =  mmsi;                                      //MMSI
            maxDraughtText.text =  maxDraught;                          //最大吃水
            vesselRegisterIdText.text = vesselRegisterId;               //登记号码  长度 14
            initialRegisterNumberText.text = initialRegisterNumber;     //初次登记号码   长度 14
            imoText.text =  imo;                                        //imo          长度 20
            madeDateText.text =  madeDate;                              //建造日期 yyyy-MM-dd         长度 10
            securityLevelText.text = securityLevel;                     //安全等级               长度 9
            widthMoudledText.text = widthMoudled;                       //型宽
            depthMoudledText.text = depthMoudled;                       //型深
            summerMeanDraftText.text = summerMeanDraft;                 //夏季满载吃水
            enterTimesText.text = enterTimes;                           //来港次数
            vesselAreaText.text =  vesselArea;                          //船舶区域分类               长度 10
            portNationText.text = portNation;                           //船籍港                     长度 30
            vesselKindText.text = vesselKind;                           //船舶种类：0：河船          长度 10
            dataSourceText.text = dataSource;                           //数据来源：0                长度 10
            remarkText.text = remark;                                   //备注                           长度 50
            updateLatestDateText.text = updateLatestDate;               //最后更新日期 yyyy-MM-dd HH:mm:ss
            vesselOperatorFidText.text = vesselOperatorFid;             //经营人FID           长度 50
            vesselOwnerFidText.text =  vesselOwnerFid;                  //所有人FID              长度 50
            vesselOwnerCnText.text =  vesselOwnerCn;                    //船舶所有人              长度 50
            vesselOwnerEnText.text = vesselOwnerEn;                     //船舶所有人英文名        长度 50
            vesselOwnerAddressText.text = vesselOwnerAddress;           //船舶所有人地址     长度 50
            operatorphoneText.text = operatorphone;                     //船舶经营人电话          长度 20
            operatoraddressText.text =  operatoraddress;                //船舶经营人地址        长度 30
            operatornameText.text =  operatorname;                      //船舶经营人名称           长度 20
            administratornameText.text =  administratorname;            //船舶管理人名称      长度 20
            administratorphoneText.text =  administratorphone;          //船舶管理人电话     长度 20
            administratoraddressText.text =   administratoraddress;     //船舶管理人地址    长度 30
            vesselOwnerPhoneText.text = vesselOwnerPhone;               //船舶所有人电话
            //vesselimageText.text = vesselimage;                         //船舶图片
        }

    }
    /*-------------------------------------------
     * 回车保存
     ---------------------------------------------*/
    Keys.onEnterPressed: shipFileModel.dealSaveVesselInfo(
                             aisIdxText.text,
                             vesselNameCnText.text,                           //中文船名  长度 30
                             vesselNameEnText.text,                           //英文船名  长度 20
                             callsignText.text,                               //呼号
                             nationText.text,                                 //国籍             长度 15
                             vesselnumberText.text,                           //船舶代号  长度 10
                              comboBox1Model.get(vesseltypeText.indexs).name,                            //船舶类型
                             passengerQuantityMaxText.text,                   //最大载客人数
                             vehicleQuantityMaxText.text,                     //最大载车人数
                             lengthText.text,                                 //船长
                             heightText.text,                                 //船高
                             widthText.text,                                  //船宽
                             maxSpeedText.text,                               //船速
                             gwtText.text,                                    //总吨
                             nwtText.text,                                    //净吨
                             anchorweightText.text,                           //锚重
                             mainEngineNumberText.text,                       //主机数量
                             mainEnginePowerText.text,                        //主机功率
                             mainEngineKindText.text,                         //主机种类 长度 20
                             dwtText.text,                                    //载重吨
                             stateText.text,                                  //当前状态  长度 3
                             mmsiText.text,                                   //MMSI
                             maxDraughtText.text,                             //最大吃水
                             vesselRegisterIdText.text,                       //登记号码  长度 14
                             initialRegisterNumberText.text,                  //初次登记号码   长度 14
                             imoText.text,                                    //imo          长度 20
                             madeDateText.text,                               //建造日期 yyyy-MM-dd         长度 10
                             securityLevelText.text,                          //安全等级               长度 9
                             widthMoudledText.text,                           //型宽
                             depthMoudledText.text,                           //型深
                             summerMeanDraftText.text,                        //夏季满载吃水
                             enterTimesText.text,                             //来港次数
                             vesselAreaText.text,                             //船舶区域分类               长度 10
                             portNationText.text,                             //船籍港                     长度 30
                             vesselKindText.text,                             //船舶种类：0：河船          长度 10
                             dataSourceText.text,                             //数据来源：0                长度 10
                             remarkText.text,                                 //备注                           长度 50
                             updateLatestDateText.text,                       //最后更新日期 yyyy-MM-dd HH:mm:ss
                             vesselOperatorFidText.text,                      //经营人FID           长度 50
                             vesselOwnerFidText.text,                         //所有人FID              长度 50
                             vesselOwnerCnText.text,                          //船舶所有人              长度 50
                             vesselOwnerEnText.text,                          //船舶所有人英文名        长度 50
                             vesselOwnerAddressText.text,                     //船舶所有人地址     长度 50
                             operatorphoneText.text,                          //船舶经营人电话          长度 20
                             operatoraddressText.text,                        //船舶经营人地址        长度 30
                             operatornameText.text,                           //船舶经营人名称           长度 20
                             administratornameText.text,                      //船舶管理人名称      长度 20
                             administratorphoneText.text,                     //船舶管理人电话     长度 20
                             administratoraddressText.text,                   //船舶管理人地址    长度 30
                             vesselOwnerPhoneText.text,                       //船舶所有人电话   长度 11
                             vesselimageText.text                             //船舶图片
                             );
    /*-------------------------------------------
     * ENTER保存
     ---------------------------------------------*/
    Keys.onReturnPressed: shipFileModel.dealSaveVesselInfo(
                              aisIdxText.text,
                              vesselNameCnText.text,                           //中文船名  长度 30
                              vesselNameEnText.text,                           //英文船名  长度 20
                              callsignText.text,                               //呼号
                              nationText.text,                                 //国籍             长度 15
                              vesselnumberText.text,                           //船舶代号  长度 10
                               comboBox1Model.get(vesseltypeText.indexs).name,                            //船舶类型
                              passengerQuantityMaxText.text,                   //最大载客人数
                              vehicleQuantityMaxText.text,                     //最大载车人数
                              lengthText.text,                                 //船长
                              heightText.text,                                 //船高
                              widthText.text,                                  //船宽
                              maxSpeedText.text,                               //船速
                              gwtText.text,                                    //总吨
                              nwtText.text,                                    //净吨
                              anchorweightText.text,                           //锚重
                              mainEngineNumberText.text,                       //主机数量
                              mainEnginePowerText.text,                        //主机功率
                              mainEngineKindText.text,                         //主机种类 长度 20
                              dwtText.text,                                    //载重吨
                              stateText.text,                                  //当前状态  长度 3
                              mmsiText.text,                                   //MMSI
                              maxDraughtText.text,                             //最大吃水
                              vesselRegisterIdText.text,                       //登记号码  长度 14
                              initialRegisterNumberText.text,                  //初次登记号码   长度 14
                              imoText.text,                                    //imo          长度 20
                              madeDateText.text,                               //建造日期 yyyy-MM-dd         长度 10
                              securityLevelText.text,                          //安全等级               长度 9
                              widthMoudledText.text,                           //型宽
                              depthMoudledText.text,                           //型深
                              summerMeanDraftText.text,                        //夏季满载吃水
                              enterTimesText.text,                             //来港次数
                              vesselAreaText.text,                             //船舶区域分类               长度 10
                              portNationText.text,                             //船籍港                     长度 30
                              vesselKindText.text,                             //船舶种类：0：河船          长度 10
                              dataSourceText.text,                             //数据来源：0                长度 10
                              remarkText.text,                                 //备注                           长度 50
                              updateLatestDateText.text,                       //最后更新日期 yyyy-MM-dd HH:mm:ss
                              vesselOperatorFidText.text,                      //经营人FID           长度 50
                              vesselOwnerFidText.text,                         //所有人FID              长度 50
                              vesselOwnerCnText.text,                          //船舶所有人              长度 50
                              vesselOwnerEnText.text,                          //船舶所有人英文名        长度 50
                              vesselOwnerAddressText.text,                     //船舶所有人地址     长度 50
                              operatorphoneText.text,                          //船舶经营人电话          长度 20
                              operatoraddressText.text,                        //船舶经营人地址        长度 30
                              operatornameText.text,                           //船舶经营人名称           长度 20
                              administratornameText.text,                      //船舶管理人名称      长度 20
                              administratorphoneText.text,                     //船舶管理人电话     长度 20
                              administratoraddressText.text,                   //船舶管理人地址    长度 30
                              vesselOwnerPhoneText.text,                       //船舶所有人电话   长度 11
                              vesselimageText.text                             //船舶图片
                              );
}
