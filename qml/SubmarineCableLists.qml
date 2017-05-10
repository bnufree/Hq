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
                            * 海缆类型
                            ---------------------------------------------*/
                            Row{
                                width: 286; height: 26; anchors.verticalCenter: parent.verticalCenter; spacing: 4;
                                Text {
                                    height: 26;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("海缆类型"); color: "#CFCFCF";
                                    font{ family: localFont.name; pointSize: 10; bold: true; }
                                }
                                ComboBox {
                                    id: aistrictCombo;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    width: 238; height: 20;
                                    ListModel { id: aistrictModel; }
                                    model: aistrictModel;
                                    Connections { target: SubmarineCableModel; onSendClearScmsCable: { aistrictModel.clear();} }
                                    Connections {
                                        target: SubmarineCableModel; onSendScmsCableByFilter: {
                                            aistrictModel.append({   "idx": idxs,                           //主键
                                                                     "createtime": createtimes,             //创建时间
                                                                     "name": names,                         //文件名
                                                                     "type": types                          //海缆类型
                                                                 } );

                                            if(aistrictCombo.idxx == 0)
                                            {
                                                aistrictCombo.text = names;
                                                aistrictCombo.idxx = idxs;
                                            }
                                        }
                                    }
                                    onSendIdx:{
                                        aistrictCombo.idxx = idx;
                                        SubmarineCableModel.setPageNo(1);
                                        SubmarineCableModel.dealFilterData( aistrictCombo.idxx, SubmarineCableModel.pageNo,SubmarineCableModel.pageSize );
                                    }
                                }
                            }
                            Item { width: 20; height: 34; }
                            /*-------------------------------------------
                            * 翻页操作
                            ---------------------------------------------*/
                            Row{
                                width: 402; height: 26;  anchors.verticalCenter: parent.verticalCenter; spacing: 3;
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
                                            SubmarineCableModel.setPageNo(1);
                                            SubmarineCableModel.dealFilterData( aistrictCombo.idxx, SubmarineCableModel.pageNo,SubmarineCableModel.pageSize );
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
                                            SubmarineCableModel.setPageNo(SubmarineCableModel.pageNo-1);
                                            SubmarineCableModel.dealFilterData( aistrictCombo.idxx, SubmarineCableModel.pageNo,SubmarineCableModel.pageSize );
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
                                            SubmarineCableModel.setPageNo(SubmarineCableModel.pageNo+1);
                                            SubmarineCableModel.dealFilterData( aistrictCombo.idxx,SubmarineCableModel.pageNo,SubmarineCableModel.pageSize );
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
                                            SubmarineCableModel.setPageNo(SubmarineCableModel.recordCount);
                                            SubmarineCableModel.dealFilterData( aistrictCombo.idxx,SubmarineCableModel.pageNo,SubmarineCableModel.pageSize );
                                        }
                                    }
                                }
                                Item { width: 20; height: 26; }
                                /*-------------------------------------------
                                * 当前页码
                                ---------------------------------------------*/
                                LineEdit{
                                    width: 50; height: 18; clip: true; color: "#010101"; anchors.verticalCenter: parent.verticalCenter;
                                    text: SubmarineCableModel.pageNo;
                                    validator: IntValidator{ bottom: 0; top: SubmarineCableModel.recordCount; }
                                    Keys.onEnterPressed: SubmarineCableModel.setPageNo(text);
                                    Keys.onReturnPressed: SubmarineCableModel.setPageNo(text);
                                }
                                Item { width: 1; height: 26; }
                                /*-------------------------------------------
                                * 总页数
                                ---------------------------------------------*/
                                Item {
                                    width: 80; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                                    Text {
                                        anchors.fill: parent;
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#CFCFCF"; text: " / "+ SubmarineCableModel.recordCount + qsTr(" 页");
                                        font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                                    }
                                }
                                Item { width: 0; height: 26; }
                                /*-------------------------------------------
                                * 总记录数
                                ---------------------------------------------*/
                                Item {
                                    width: 150; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                                    Text {
                                        anchors.fill: parent;
                                        verticalAlignment: Text.AlignVCenter
                                        color: "#CFCFCF"; text: "海缆节点总数: "+ SubmarineCableModel.rowCount + qsTr(" 点");
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
                                    id : closeMouseArea; anchors.fill: parent; hoverEnabled: true; onClicked: SubmarineCableModel.sendInfoPanelsVisible(false);
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
                    tableTitle: ["序号","海缆编号","纬度","经度","KP","实际埋深（m）","目标埋深（m）","埋深差值（m）","抗击锚重（kg）", "抛石保护","备注"];
                    scrollWidth: 11*columnWidth;
                    delegate: topTableDelegate;
                    model: topTableModel;
                    /*-------------------------------------------
                     * 数据模型
                     ---------------------------------------------*/
                    ListModel { id: topTableModel; }
                    /*-------------------------------------------
                     * 清理数据
                     ---------------------------------------------*/
                    Connections { target: SubmarineCableModel; onSendClearAll: { topTableModel.clear();} }
                    /*-------------------------------------------
                     * 向模型添加数据
                     ---------------------------------------------*/
                    Connections {
                        target: SubmarineCableModel; onSendCableDataByFilter: {
                            topTableModel.append({   "item0": idx,                  //序号
                                                     "item1": code,                  //编号
                                                     "item2": lat,                   //纬度X
                                                     "item3": lon,                   //经度Y
                                                     "item4": kp,                    //KP值
                                                     "item5": realdepth,             //实际埋深
                                                     "item6": targetdepth,           //目标埋深
                                                     "item7": depthgap,              //埋深差值
                                                     "item8": anchor,                //抗击锚重
                                                     "item9": riprap,                //抛石保护
                                                     "item10": remark,               //备注
                                                 } );
                        }
                    }
                    /*-------------------------------------------
                    * 代理
                    ---------------------------------------------*/
                    Component {
                        id: topTableDelegate;
                        Rectangle{
                            width: tablesView.width-2; height: rowHeight;
                            color:ListView.isCurrentItem ? "#8edcf3" : "#FFFFFF";
                            border{ width: 1; color: "#dedede" }
                            Row{
                                anchors.fill: parent;
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
                            }
                            /*-------------------------------------------
                             * 响应鼠标点击项动作
                             ---------------------------------------------*/
                            MouseArea {
                                anchors.fill: parent;
                                onClicked: { roots.setComboboxClose();tableView.currentIndex = index;}
                            }
                        }
                    }
                }
            }

        }
    }
}
