/****************************************************************************
**
* @Project : Frameworks
* @File: tableView.qml
* @Description:
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
    property int rowHeight: 31;
    property int ii: 0;
    property int currentPage: 0;

    function setComboboxClose(){
        alarmTypeCombo.state = "";
        alarmLevelCombo.state = "";
    }
    Rectangle{
        anchors.fill: parent; color: Qt.rgba(0.06,0.31,0.55,0.8);
        Column{
            anchors.fill: parent;
            Rectangle{ width: parent.width; height: 1; color: "#818181"; }
            /*-------------------------------------------
             * 标题区域
             ---------------------------------------------*/
            Rectangle{
                MouseArea{ anchors.fill: parent; onClicked:roots.setComboboxClose();}
                id: titleRect; width: parent.width; height: 39; color: Qt.rgba(0.06,0.31,0.55,0.8); z: 1;
                Rectangle{
                    anchors.fill: parent; color: Qt.rgba(0.06,0.31,0.55,0.8);
                    Row{
                        anchors.fill: parent;
                        Item { width: 6; height: 34; }
                        /*-------------------------------------------
                        * 告警类型
                        ---------------------------------------------*/
                        Row{
                            width: 186; height: 26; anchors.verticalCenter: parent.verticalCenter;
                            Text {
                                width: 60; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("告警类型"); color: "#CFCFCF";
                                font{ family: localFont.name; pointSize: 10; bold: true; }
                            }
                            ComboBox {
                                id: alarmTypeCombo;
                                anchors.verticalCenter: parent.verticalCenter;
                                width: 128; height: 20;
                                ListModel {
                                    id: comboBox1Model
                                    ListElement { idx: "0"; name: "全部类型"; }
                                    ListElement { idx: "1"; name: "抛锚预警"; }
                                    ListElement { idx: "2"; name: "搁浅预警"; }
                                }
                                model: comboBox1Model;
                                text: comboBox1Model.get(0).name; 
                                onSendIndex: {
                                    alarmEventModel.setPageNo(1);
                                    alarmEventModel.dealFilterAlarm( true,alarmTypeCombo.text, alarmLevelCombo.indexs, "");
                                }
                            }
                        }
                        Item { width: 20; height: 34; }
                        /*-------------------------------------------
                        * 告警级别
                        ---------------------------------------------*/
                        Row{
                            width: 186; height: 26; anchors.verticalCenter: parent.verticalCenter;
                            Text {
                                width: 60; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("告警级别"); color: "#CFCFCF";
                                font{ family: localFont.name; pointSize: 10; bold: true; }
                            }
                            ComboBox {
                                id: alarmLevelCombo;
                                anchors.verticalCenter: parent.verticalCenter;
                                width: 128; height: 20;
                                ListModel {
                                    id: comboBox2Model
                                    ListElement { idx: "0"; name: "全部预警"; }
                                    ListElement { idx: "1"; name: "一级预警"; }
                                    ListElement { idx: "2"; name: "二级预警"; }
                                    ListElement { idx: "3"; name: "三级预警"; }
                                    ListElement { idx: "4"; name: "四级预警"; }
                                    ListElement { idx: "5"; name: "五级预警"; }
                                }
                                model: comboBox2Model;
                                text: comboBox2Model.get(0).name;
                                onSendIndex: {
                                    clearTable();
                                    alarmEventModel.setPageNo(1);
                                    alarmEventModel.dealFilterAlarm( false,alarmTypeCombo.text, alarmLevelCombo.indexs, "");
                                }
                            }
                        }
                        Item { width: 20; height: 34; }
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
                                        clearTable();
                                        alarmEventModel.setPageNo(1);
                                        alarmEventModel.dealFilterAlarmEvent( alarmEventModel.pageNo,alarmEventModel.pageSize );
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
                                        clearTable();
                                        alarmEventModel.setPageNo(alarmEventModel.pageNo-1);
                                        alarmEventModel.dealFilterAlarmEvent( alarmEventModel.pageNo,alarmEventModel.pageSize );
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
                                        clearTable();
                                        alarmEventModel.setPageNo(alarmEventModel.pageNo+1);
                                        alarmEventModel.dealFilterAlarmEvent( alarmEventModel.pageNo,alarmEventModel.pageSize );
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
                                        clearTable();
                                        alarmEventModel.setPageNo(alarmEventModel.recordCount);
                                        alarmEventModel.dealFilterAlarmEvent( alarmEventModel.pageNo,alarmEventModel.pageSize );
                                    }
                                }
                            }
                            Item { width: 20; height: 26; }
                            /*-------------------------------------------
                            * 当前页码
                            ---------------------------------------------*/
                            LineEdit{
                                width: 50; height: 18; clip: true; color: "#010101"; anchors.verticalCenter: parent.verticalCenter;
                                text: alarmEventModel.pageNo;
                                validator: IntValidator{ bottom: 0; top: alarmEventModel.recordCount; }
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
                                    color: "#CFCFCF"; text: " / "+ alarmEventModel.recordCount + qsTr(" 页");
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                                }
                            }
                            Item { width: 10; height: 26; }
                            /*-------------------------------------------
                            * 总记录数
                            ---------------------------------------------*/
                            Item {
                                width: 100; height: 26; anchors.verticalCenter: parent.verticalCenter; clip: true;
                                Text {
                                    anchors.fill: parent;
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#CFCFCF"; text: "告警总数: "+ alarmEventModel.rowCount + qsTr(" 条");
                                    font{ family: localFont.name; pointSize: 10; bold: true; underline: false; }
                                }
                            }
                        }

                    }
                    Item {
                        width: 36; height: parent.height;
                        anchors.right: parent.right; //anchors.rightMargin: 2;
                        anchors.verticalCenter: parent.verticalCenter;
                        BorderImage {
                            width: closeMouseArea.containsMouse ? 16: 14; height: closeMouseArea.containsMouse ? 16: 14; smooth: true;
                            anchors.centerIn: parent;
                            anchors.verticalCenterOffset: -2;
                            border { left: 1; top: 1; right: 1; bottom: 1 }
                            horizontalTileMode: BorderImage.Stretch;
                            verticalTileMode: BorderImage.Stretch;
                            source: "qrc:/core/qml/image/closebutton.png";
                            MouseArea{
                                id : closeMouseArea; anchors.fill: parent; hoverEnabled: true; onClicked: {roots.setComboboxClose();alarmEventModel.sendInfoPanelsVisible(false);}
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
                    tableTitle: ["告警序号","告警类型","告警级别","预警值","核心风险值","辅助风险值","目标名","目标id","坐标","速度(节)","位置","告警时间","预案名称"]
                    scrollWidth: 13*columnWidth;
                    delegate: topTableDelegate;
                    model: topTableModel;
                    isorderimg: true;
                    onSendOrder: {
                        if( indexx == 2 )
                        {
                            alarmEventModel.setAlarmEventSort( true, isSort);
                        }else if( indexx == 11 )
                        {
                            alarmEventModel.setAlarmEventSort( false, isSort);
                        }
                    }
                    /*-------------------------------------------
                     * 数据模型
                     ---------------------------------------------*/
                    ListModel {
                        id: topTableModel;
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                        ListElement {item0: "";item1: "";item2: "";alarmNum: 0; item4: "";item5: "";item6: "";item7: "";item8: "";item9: "";item10: "";item11: "";item12: "";item14: "";m_alarmid:""}
                    }
                    /*-------------------------------------------
                     * 重值模型
                     ---------------------------------------------*/
                    Connections { target: alarmEventModel; onSendInitialList: { clearTable(); } }
                    /*-------------------------------------------
                     * 向模型添加数据
                     ---------------------------------------------*/
                    Connections {
                        target: alarmEventModel;
                        onSendAlarmEvent: {
                            if(searchString(alarmtype)){
                                return;
                            }

                            var alarmNum = alarmTypeNum;

                            if(idx === ""){
                                alarmNum = 0;
                            }
                            topTableModel.set( ids, {
                                                  "item0":idx,              //编号
                                                  "item1":alarmname,        //告警名称  0<长度<8
                                                  "item2":alarmtype,        //告警类型  0<长度< 8
                                                  "alarmNum":alarmNum/*alarmTypeNum*/,  //告警 值
                                                  "item4":alarmvalue,       //预警值
                                                  "item5":riskvalue,        //风险值
                                                  "item6":riskweight,       //风加权险值
                                                  "item7":targetname,       //目标名    0<长度< 20
                                                  "item8":code,             //编号:船舶mmsi
                                                  "item9":coordinate,       //位置 经度，纬度
                                                  "item10":	sog,            //速度
                                                  "item11":location,        //位置 0<长度< 20
                                                  "item12":alarmtime,       //告警时间
                                                  "item14":emergency,       //预案 0<长度<10
                                                  "m_alarmid":alarmid       //告警ID
                                              } );


                        }
                    }
                    /*-------------------------------------------
                    * 代理
                    ---------------------------------------------*/
                    Component {
                        id: topTableDelegate;
                        Rectangle{
                            id:rectangleCurren
                            width: tableView.width; height: rowHeight;
                            color:/*ListView.isCurrentItem ? "#8edcf3" : */ searchString(item2)?"#cccccc": alarmColor[alarmNum];
                            border{ width: 1; color: "#dedede" }
                            Row{
                                anchors.centerIn: parent;
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item0; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item1; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item2; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item4; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item5; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item6; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item7; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item8; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item9; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item10; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item11; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item12; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                                Item {
                                    width: columnWidth; height: rowHeight; clip: true;
                                    Text {
                                        anchors.centerIn: parent;
                                        text: item14; color: (tablesView.currentIndex == index) ? "#004b5f" : ( (alarmNum==4) ? "#FCFCFC" : "#202020" );
                                        font{ family: localFont.name; pointSize: 9; }
                                    }
                                }
                            }
                            /*-------------------------------------------
                             * 响应鼠标点击项动作
                             ---------------------------------------------*/
                            MouseArea {
                                anchors.fill: parent;
                                onClicked: { roots.setComboboxClose();tableView.currentIndex = index; alarmEventModel.sendAlarmDetailById(m_alarmid); alarmEventModel.sendAttentionMMIS( item8 ) }
                                onDoubleClicked: {
                                    alarmEventModel.sendEcdisPositionMmis( item8 );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    function clearTable()
    {
        topTableModel.clear();
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
        topTableModel.append({ "item0": "","item1": "", "item2": "","alarmNum": 5,"item4": "","item5": "","item6": "","item7": "","item8": "","item9": "","item10": "","item11": "","item12": "","item14": "","m_alarmid":"" } )
    }
    function searchString(m_string)
    {
        if(m_string.indexOf("过期") !== -1)
        {
            return true;
        }
        else{
            return false;
        }
    }
}
