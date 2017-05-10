/****************************************************************************
**
* @Project : Frameworks
* @File: tableView.qml
* @Description: 窗口顶部table
* @Copyright: Copyright (c) 2013
* @Company: 深圳置辰海信科技有限公司
* @WebSite: http://www.szcenterstar.com/
* @author:
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
import "Components"
Item{
    id: roots;
    width: 1600; height: 42;
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int columnWidth: 89;
    property int rowHeight: 31;
    property int ii: 0;
    property int currentPage: 0;

    function setComboboxClose(){
        aistrictCombo.state = "";
        shipTypeCombo.state = "";
    }

    Rectangle{
        anchors.fill: parent; color: Qt.rgba(0.06,0.31,0.55,0.8);
        Column{
            Rectangle{ width: parent.width; height: 1; color: "#818181"; }
            anchors.fill: parent;
            /*-------------------------------------------
             * 标题区域
             ---------------------------------------------*/
            Rectangle{
                MouseArea{ anchors.fill: parent; onClicked: roots.setComboboxClose();}
                id: titleRect; width: parent.width; height: 39; color: Qt.rgba(0.06,0.31,0.55,0.8); z: 1;
                Rectangle{
                    anchors.fill: parent; color: Qt.rgba(0.06,0.31,0.55,0.8);
                    Row {
                        anchors.fill: parent;
                        Row{
                            width: parent.width-closeButtons.width;  height: parent.height;
                            Item { width: 6; height: 34; }
                            /*-------------------------------------------
                            *
                            ---------------------------------------------*/
                            TabButton{
                                id: tabListView;
                                anchors.verticalCenter: parent.verticalCenter;
                            }

                            Item { width: 8; height: 34; }
                            /*-------------------------------------------
                            * 搜索框
                            ---------------------------------------------*/
                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter;
                                width: 194; height: 22; color: "#e2e2e2";
                                radius: 2; smooth: true; clip: true;
                                border { width: 1; color:"#727272";  }
                                Text {
                                    id: typeSomething; visible: false;
                                    anchors.fill: parent; anchors.leftMargin: 10
                                    verticalAlignment: Text.AlignVCenter;
                                    color: "#1a1a1a"; text: "Search";
                                    font{ family: localFont.name; pointSize: 8; bold: false; italic: false; }
                                    smooth: true;
                                }
                                TextInput {
                                    id: searchInput
                                    anchors{ verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 10; rightMargin: 12; }
                                    anchors.verticalCenterOffset: 1;
                                    width: parent.width-46; color: "#1a1a1a";
                                    font{ family: localFont.name; pointSize: 9; bold: false; italic: false; }
                                    smooth: true; focus: true; cursorVisible: true; selectByMouse: true;
                                    text:"";
                                    onTextChanged: { shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text); }
                                    Keys.onEnterPressed:  { shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text); }
                                    Keys.onReturnPressed:  { shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text); }
                                }
                                BorderImage {
                                    id: searchBtn;
                                    anchors{ verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 12; }
                                    width: 13; height: 14; smooth: true;
                                    border { left: 3; top: 3; right: 3; bottom: 3 }
                                    horizontalTileMode: BorderImage.Repeat;
                                    verticalTileMode: BorderImage.Repeat;
                                    source: "qrc:/core/qml/image/tab_search.png";
                                    MouseArea {  anchors.fill: parent; onClicked: roots.setComboboxClose();}
                                }
                                states: State { name: "hasText"; when: searchInput.text != '';  PropertyChanges { target: typeSomething; opacity: 0 }  }
                                transitions: [
                                    Transition { from: ""; to: "hasText"; NumberAnimation { exclude: typeSomething; properties: "opacity" } },
                                    Transition { from: "hasText"; to: ""; NumberAnimation { properties: "opacity" } }
                                ]
                            }
                            Item { width: 60; height: 34; }
                            /*-------------------------------------------
                            * 管辖区
                            ---------------------------------------------*/
                            Row{
                                width: 186; height: 26; anchors.verticalCenter: parent.verticalCenter;
                                Text {
                                    width: 46; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("管辖区"); color: "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; }
                                }
                                ComboBox {
                                    id: aistrictCombo;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    width: 128; height: 20;
                                    ListModel {
                                        id: aistrictModel
                                        ListElement { idx: "0"; name: "全部区域"; }
                                        ListElement { idx: "1"; name: "监控区域"; }
                                    }
                                    model: aistrictModel;
                                    text: aistrictModel.get(0).name;
                                    Connections{ target: shipInforModel; onSendClearDistrict: { aistrictModel.clear(); aistrictModel.append( {"idx": 0,"name": qsTr("全部区域")});
                                                                                                                        aistrictModel.append({"idx": 1,"name": qsTr("监控区域")})} }
                                    Connections{ target: shipInforModel; onSendDistrictUpdate: {
                                            aistrictModel.append( {"idx": qsTr(idx),"name": qsTr(name)} ) ;
                                            /*-------------------------------------------
                                            * 重新查询
                                            ---------------------------------------------*/
                                            shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text);
                                        }
                                    }
                                    onSendIndex: {
                                        shipInforModel.setPageNo(1);
                                        shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text);
                                    }
                                }
                            }
                            Item { width: 6; height: 34; }
                            /*-------------------------------------------
                            * 船舶类型
                            ---------------------------------------------*/
                            Row{
                                width: 186; height: 26; anchors.verticalCenter: parent.verticalCenter;
                                Text {
                                    width: 58; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("数据类型"); color: "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; }
                                }
                                ComboBox {
                                    id: shipTypeCombo;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    width: 128; height: 20;
                                    ListModel {
                                        id: shipTypeModel
                                        ListElement { idx: "0"; name: "全部类型"; }
                                        ListElement { idx: "1"; name: "A类船"; }
                                        ListElement { idx: "2"; name: "BCS类船"; }
                                        ListElement { idx: "3"; name: "BSO类船"; }
                                    }
                                    model: shipTypeModel;
                                    text: shipTypeModel.get(0).name;
                                    onSendIndex:{
                                        shipInforModel.setPageNo(1);
                                        shipInforModel.dealFilterList( tabListView.indexs,shipTypeCombo.indexs,searchInput.text,aistrictCombo.text);
                                    }
                                }
                            }
                            Item { width: 40; height: 34; }
                            /*-------------------------------------------
                            * 翻页操作
                            ---------------------------------------------*/
                            Row{
                                width: 402; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                Text {
                                    width: 41; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr(" 首页 "); color: fristPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                                    MouseArea{
                                        id : fristPageMouseArea;
                                        anchors.fill: parent; hoverEnabled: true;
                                        onClicked: {
                                            roots.setComboboxClose();
                                            shipInforModel.setPageNo(1);
                                            shipInforModel.dealFilterAisData( shipInforModel.pageNo,shipInforModel.pageSize );
                                        }
                                    }
                                }
                                Item { width: 3; height: 26; }
                                Text {
                                    width: 46; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("上一页"); color: previousPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                                    MouseArea{
                                        id : previousPageMouseArea;
                                        anchors.fill: parent; hoverEnabled: true;
                                        onClicked: {
                                            roots.setComboboxClose();
                                            shipInforModel.setPageNo(shipInforModel.pageNo-1);
                                            shipInforModel.dealFilterAisData( shipInforModel.pageNo,shipInforModel.pageSize );
                                        }
                                    }
                                }
                                Item { width: 3; height: 26; }
                                Text {
                                    width: 46; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("下一页"); color: nextPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                                    MouseArea{
                                        id : nextPageMouseArea;
                                        anchors.fill: parent; hoverEnabled: true;
                                        onClicked: {
                                            roots.setComboboxClose();
                                            shipInforModel.setPageNo(shipInforModel.pageNo+1);
                                            shipInforModel.dealFilterAisData( shipInforModel.pageNo,shipInforModel.pageSize );
                                        }
                                    }
                                }
                                Item { width: 3; height: 26; }
                                Text {
                                    width: 41; height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr(" 尾页 "); color: endPageMouseArea.containsMouse ? "#F79D00" : "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: true; }
                                    MouseArea{
                                        id : endPageMouseArea;
                                        anchors.fill: parent; hoverEnabled: true;
                                        onClicked: {
                                            roots.setComboboxClose();
                                            shipInforModel.setPageNo(shipInforModel.recordCount);
                                            shipInforModel.dealFilterAisData( shipInforModel.pageNo,shipInforModel.pageSize );
                                        }
                                    }
                                }
                                Item { width: 20; height: 26; }
                                /*-------------------------------------------
                                * 当前页码
                                ---------------------------------------------*/
                                LineEdit{
                                    width: 50; height: 18; clip: true; color: "#010101"; anchors.verticalCenter: parent.verticalCenter;
                                    text: shipInforModel.pageNo;
                                    validator: IntValidator{ bottom: 0; top: shipInforModel.recordCount; }
                                    Keys.onEnterPressed: shipInforModel.setPageNo(text);
                                    Keys.onReturnPressed: shipInforModel.setPageNo(text);
                                }
                                Item { width: 1; height: 26; }
                                /*-------------------------------------------
                                * 总页数
                                ---------------------------------------------*/
                                Item {
                                    width: 60; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                                    Text {
                                        anchors.fill: parent;
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#CFCFCF"; text: " / "+ shipInforModel.recordCount + qsTr(" 页");
                                        font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                                    }
                                }
                                Item { width: 0; height: 26; }
                                /*-------------------------------------------
                                * 总记录数
                                ---------------------------------------------*/
                                Item {
                                    width: 130; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                                    Text {
                                        anchors.fill: parent;
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#CFCFCF"; text: "船舶总数: "+ shipInforModel.rowCount + qsTr(" 条");
                                        font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                                    }
                                }
                            }
                        }
                        Item {
                            id: closeButtons;
                            width: 36; height: parent.height;
                            BorderImage {
                                width: closeMouseArea.containsMouse ? 16: 14; height: closeMouseArea.containsMouse ? 16: 14; smooth: true;
                                anchors.centerIn: parent;
                                anchors.verticalCenterOffset: -4;
                                border { left: 1; top: 1; right: 1; bottom: 1 }
                                horizontalTileMode: BorderImage.Stretch;
                                verticalTileMode: BorderImage.Stretch;
                                source: "qrc:/core/qml/image/closebutton.png";
                                MouseArea{
                                    id : closeMouseArea; anchors.fill: parent; hoverEnabled: true; onClicked: {roots.setComboboxClose();shipInforModel.sendInfoPanelsVisible(false);}
                                }
                            }
                        }
                    }

                }
            }
            Item {
                width: parent.width; height: parent.height-titleRect.height;
                /*-------------------------------------------
                 * 表格
                 ---------------------------------------------*/
                TableWidget{
                    id: tablesView;
                    anchors.fill: parent;
                    tableTitle: ["船舶序号","船名","呼号","MMSI","IMO编码","管辖区","船舶类型","航行状态","船长(米)","船宽(米)","吃水(米)","锚重(吨)","坐标","船艏向","航迹向","转向率","航速(节)","目的地","更新时间","过航次数","船舶标注"]
                    scrollWidth: 21*columnWidth;
                    delegate: topTableDelegate;
                    model: topTableModel;
                    /*-------------------------------------------
                     * 数据模型
                     ---------------------------------------------*/
                    ListModel {
                        id: topTableModel;
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                        ListElement {item0: "";item1: "";item2: ""; item3: "";item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item13: "";item14: "";item15: "";item16: "";item17: "";item18: "";item19: "";item20: "";}
                    }
                    /*-------------------------------------------
                     * 重值模型
                     ---------------------------------------------*/
                    Connections { target: shipInforModel; onSendInitialList: {
                            topTableModel.clear();
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                            topTableModel.append({ "item0": "","item1": "", "item2": "","item3": "","item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item13": "","item14": "","item15": "","item16": "","item17": "","item18": "","item19": "","item20": "" } )
                        }
                    }
                    /*-------------------------------------------
                     * 向模型添加数据
                     ---------------------------------------------*/
                    Connections {
                        target: shipInforModel; onSendAisShipData: {
                            topTableModel.set( idx, {
                                                  "item0": ids,
                                                  "item1": ship1,
                                                  "item2": ship2,
                                                  "item3": ship3,
                                                  "item4": ship4,
                                                  "item5": ship5,
                                                  "item6": ship6,
                                                  "item7": ship7,
                                                  "item8": ship8,
                                                  "item9": ship9,
                                                  "item10": ship10,
                                                  "item11": ship11,
                                                  "item12": ship12,
                                                  "item13": ship13,
                                                  "item14": ship14,
                                                  "item15": ship15,
                                                  "item16": ship16,
                                                  "item17": ship17,
                                                  "item18": ship18,
                                                  "item19": ship19,
                                                  "item20": ship20
                                              } );
                        }
                    }
                    /*-------------------------------------------
                    * 代理
                    ---------------------------------------------*/
                    Component {
                        id: topTableDelegate;
                        Rectangle{
                            width: tableView.width; height: rowHeight;
                            color:ListView.isCurrentItem ? "#8edcf3" : "#FFFFFF";
                            border{ width: 1; color: "#dedede" }
                            Row{
                                anchors.centerIn: parent;
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item0; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item1; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item2; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item3; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item4; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item5; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item6; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item7; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item8; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item9; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item10; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item11; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item12; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item13; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item14; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item15; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item16; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item17; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item18; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item19; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item20; color: "#202020";
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 响应鼠标点击项动作
                             ---------------------------------------------*/
                            MouseArea {
                                anchors.fill: parent;
                                onClicked: { roots.setComboboxClose();tableView.currentIndex = index; shipInforModel.dealAttentionMMIS( item3 );}
                                onDoubleClicked: shipInforModel.sendEcdisPositionMmis( item3 );
                            }
                        }
                    }
                }
            }

        }
    }
}
