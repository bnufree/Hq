import QtQuick 1.1
import "Components"
Rectangle {
    width: 100; height: 62; color: "#ffffff";
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    property int alarmTotal:0;
    property int deletIdx: 0;
    Column{
        anchors.fill:parent; clip: true;
        TableWidget{
            id: tablesView; clip: true;
            width: parent.width; height: parent.height-30;
            tableTitle: ["告警序号","目标id","告警内容","告警级别","告警时间","确定"]
            scrollWidth: 6*columnWidth;
            delegate: topTableDelegate;
            model: topTableModel;
            columnWidth: 131;
            ListModel { id: topTableModel; }
            //Connections { target: AlarmEventDialogModel; onSendClearList: { topTableModel.clear(); } }
            Connections {
                target: AlarmEventDialogModel;
                onSendClearList: { topTableModel.clear(); tablesView.isDisplayVBar = false;}
                onSendAlarmInfo: {
                    //查找对应的id，然后更新，否则直接添加
                    var found = false;
                    for(var i=0; i<topTableModel.count; i++)
                    {
                        if(topTableModel.get(i).item0 === idx)
                        {
                            topTableModel.set(i, { "item0":idx,
                                                  "item1":alarmcontent,
                                                  "item2":alarmtype,
                                                  "item3":alarmtime,
                                                  "item4":""
                                              })
                            found = true;
                            break;
                        }
                    }

                    if(!found)
                        topTableModel.append( { "item0":idx,
                                                 "item1":alarmcontent,
                                                 "item2":alarmtype,
                                                 "item3":alarmtime,
                                                 "item4":""
                                             } );
                    alarmTotal = total;
                }
                onDeleteAlarmInfo: {
                    //查找对应的id，然后移除
                    for(var i=0; i<topTableModel.count; i++)
                    {
                        if(topTableModel.get(i).item0 === idx)
                        {
                            topTableModel.remove(i);
                            break;
                        }
                    }
                }
            }
            /*-------------------------------------------
            * 代理
            ---------------------------------------------*/
            Component {
                id: topTableDelegate;
                Rectangle{
                    width: tableView.width; height: rowHeight; color:"#fefefe";
                    border{ width: 1; color: "#f1f1f1" }
                    MouseArea {
                        anchors.fill: parent;
                        onDoubleClicked:{
                            AlarmEventDialogModel.slotCurrenMMSI(topTableModel.get(index).item0);
                            AlarmEventDialogModel.slotCurrenVisibls();
                        }
                    }
                    Row{
                        anchors.centerIn: parent;
                        Item {
                            width: columnWidth; height: rowHeight; clip: true;
                            Text {
                                anchors.centerIn: parent;
                                text: index+1; color: "#202020";
                                font{ family: localFont.name; pointSize: 9; }
                            }
                        }
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
                        Item{
                            width: columnWidth; height: rowHeight; clip: true;
                            Text {
                                id: indexValue
                                visible: false;
                                text: item0;
                            }
                            PushButtons {
                                anchors.centerIn: parent;
                                width: 85;  height: 20;
                                label: qsTr("确定");
                                onClicked: {
                                    AlarmEventDialogModel.slotDeleteData(item0,item2);
                                }
                            }
                        }
                    }
                    /*-------------------------------------------
                     * 响应鼠标点击项动作
                     ---------------------------------------------*/

                }
            }
        }
        Item{ width: parent.width; height: 5; }
        Row
        {
            anchors.right: parent.right; anchors.rightMargin: 60;
            Text {
                anchors.verticalCenter: parent.verticalCenter;
                text: "关闭对话框，5分钟后重新提示报警信息。"; color: "#818181";
                font{ family: localFont.name; pointSize: 10; }
            }
            Item { width: 30; height: parent.height; }
            PushButtons {
                 anchors.verticalCenter: parent.verticalCenter;
                width: 85;  height: 20;
                label: qsTr("全部确定")
                onClicked: {
                    AlarmEventDialogModel.reject();
                }
            }
        }
    }
}
