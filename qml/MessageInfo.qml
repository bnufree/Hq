import QtQuick 1.1
import "Components"

Rectangle {
    width: 600; height: 500; color: "#ffffff";
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }
    property string idx: "";
    property int columnWidth: 89;
    property int widgetColumnWidth: 100;
    Column{
        width: parent.width - 2; height:parent.height;spacing: 5;
        Item{
            id:rectangle1
            width: parent.width; height:30;
            Row{
                width: parent.width; height: 28 ;
                Text {
                    id:searchText
                    width: 80; height: 26;
                    color: "#333333";
                    anchors.verticalCenter: parent.verticalCenter;
                    text: qsTr("  收件人");
                    font{ family: localFont.name; pointSize: 11; bold: false; }
                }
                LineEditInput{
                    id: searchTextInput;
                    width: 300; height: 24;
                    anchors.verticalCenter: parent.verticalCenter;
                    enabled: false;
                    onTextChanged: {
                    }
                }
            }
        }
        Item{
            id:rectangle2
            width: parent.width- 2; height:30;
            Row{
                width: parent.width; height:28;
                Text {
                    id:phoneText
                    width: 80; height: 26;
                    color: "#333333";
                    anchors.verticalCenter: parent.verticalCenter;
                    text: qsTr("  联系电话");
                    font{ family: localFont.name; pointSize: 11; bold: false; }
                }
                LineEditInput{
                    id: phoneTextInput;
                    width: 300; height: 24;
                    anchors.verticalCenter: parent.verticalCenter;
                    enabled: false;
                    onTextChanged: {
                    }
                }
            }
        }
        Item{width: parent.height; height: 2;}
        Rectangle{
            id:rectangle3
            width: parent.width; height: 280;
            color:"#ffffff"
            border{ width: 1; color: "#e8e2e2"; }
            clip: true;
            Column{
                width: parent.width -4; height: 260;
                anchors.margins: 4;
                spacing: 1;
                Item{
                    id:historyText
                    width: parent.width - 5; height: 28; clip: true;
                    Text{
                        anchors.leftMargin: 6;
                        color:"#333333";
                        anchors.verticalCenter: parent.verticalCenter;
                        text: qsTr("  历史记录");
                        font{ family: localFont.name; pointSize: 11; bold: true; }
                    }
                }
                Rectangle{width: parent.width+4; height: 1; color:"#e8e2e2";}
                Item{
                    width: parent.width - 5; height: rectangle3.height - historyText.height;clip: true;
                    anchors.leftMargin: 6;
                    Column{
                        width: parent.width;  height: parent.height;
                        /*-------------------------------------------
                        * 翻页操作
                        ---------------------------------------------*/
                        Row{
                            width: 500; height: 26;
                            Text {
                                width: 41; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr(" 首页 "); color: fristPageMouseArea.containsMouse ? "#F79D00" : "#333333";
                                font{ family: localFont.name; pointSize: 10;  underline: true; }
                                MouseArea{
                                    id : fristPageMouseArea;
                                    anchors.fill: parent; hoverEnabled: true;
                                    onClicked: {
                                        messageManageModel.setPageNo(1);
                                        messageManageModel.getGPRSMessageByFilter( messageManageModel.pageNo,messageManageModel.pageSize,idx );
                                    }
                                }
                            }
                            Item { width: 3; height: 26; }
                            Text {
                                width: 46; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("上一页"); color: previousPageMouseArea.containsMouse ? "#F79D00" : "#333333";
                                font{ family: localFont.name; pointSize: 10;  underline: true; }
                                MouseArea{
                                    id : previousPageMouseArea;
                                    anchors.fill: parent; hoverEnabled: true;
                                    onClicked: {
                                        messageManageModel.setPageNo(messageManageModel.pageNo-1);
                                        messageManageModel.getGPRSMessageByFilter( messageManageModel.pageNo,messageManageModel.pageSize,idx );
                                    }
                                }
                            }
                            Item { width: 3; height: 26; }
                            Text {
                                width: 46; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr("下一页"); color: nextPageMouseArea.containsMouse ? "#F79D00" : "#333333";
                                font{ family: localFont.name; pointSize: 10;  underline: true; }
                                MouseArea{
                                    id : nextPageMouseArea;
                                    anchors.fill: parent; hoverEnabled: true;
                                    onClicked: {
                                        messageManageModel.setPageNo(messageManageModel.pageNo+1);
                                        messageManageModel.getGPRSMessageByFilter( messageManageModel.pageNo,messageManageModel.pageSize,idx );
                                    }
                                }
                            }
                            Item { width: 3; height: 26; }
                            Text {
                                width: 41; height: 26;
                                anchors.verticalCenter: parent.verticalCenter;
                                verticalAlignment: Text.AlignVCenter
                                text: qsTr(" 尾页 "); color: endPageMouseArea.containsMouse ? "#F79D00" : "#333333";
                                font{ family: localFont.name; pointSize: 10;  underline: true; }
                                MouseArea{
                                    id : endPageMouseArea;
                                    anchors.fill: parent; hoverEnabled: true;
                                    onClicked: {
                                        messageManageModel.setPageNo(messageManageModel.recordCount);
                                        messageManageModel.getGPRSMessageByFilter( messageManageModel.pageNo,messageManageModel.pageSize,idx );
                                    }
                                }
                            }
                            Item { width: 20; height: 26; }
                            /*-------------------------------------------
                            * 当前页码
                            ---------------------------------------------*/
                            LineEdit{
                                width: 50; height: 18; clip: true; color: "#010101"; anchors.verticalCenter: parent.verticalCenter;
                                text: messageManageModel.pageNo;
                                validator: IntValidator{ bottom: 0; top: messageManageModel.recordCount; }
                                Keys.onEnterPressed: messageManageModel.setPageNo(text);
                                Keys.onReturnPressed: messageManageModel.setPageNo(text);
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
                                    color: "#333333"; text: " / "+ messageManageModel.recordCount + qsTr(" 页");
                                    font{ family: localFont.name; pointSize: 10;  underline: false; }
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
                                    color: "#333333"; text: "历史消息总数: "+ messageManageModel.rowCount + qsTr(" 条");
                                    font{ family: localFont.name; pointSize: 10; underline: false; }
                                }
                            }
                        }
                        Rectangle{
                            width: parent.width;  height: parent.height - 30;
                            TableWidget{
                                id: tidalTablesView;
                                width: parent.width; height: parent.height -5;
                                tableTitle: ["发送消息","发送时间"];
                                scrollWidth: 2*columnWidth;
                                columnWidth:widgetColumnWidth*2;
                                delegate: tidalTableDelegate;
                                model: tidalTableModel;
                                /*-------------------------------------------
                                 * 数据模型
                                 ---------------------------------------------*/
                                ListModel { id: tidalTableModel; }
                                Connections { target: messageManageModel; onSendClearTidalTable: tidalTableModel.clear(); }
                                /*-------------------------------------------
                                * 代理
                                ---------------------------------------------*/
                                Component {
                                    id: tidalTableDelegate;
                                    Rectangle{
                                        width: tidalTablesView.width;
                                        height: rowHeight;
                                        color:ListView.isCurrentItem ? "#8edcf3" : "#FFFFFF";
                                        border{ width: 1; color: "#dedede" }
                                        Row{
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
                                        }
                                        /*-------------------------------------------
                                         * 响应鼠标点击项动作
                                         ---------------------------------------------*/
                                        MouseArea {
                                            anchors.fill: parent;
                                            onClicked: {
                                                tableView.currentIndex = index;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Item{width: parent.height; height: 2;}
        Rectangle{
            width: parent.width; height: 160;clip: true;
            color:"#ffffff"
            border{ width: 1; color: "#e8e2e2"; }
            Column{
                width: parent.width -4; height: 160;
                anchors.margins: 4;
                spacing: 1;
                Item{
                    width: parent.width - 5; height: 28; clip: true;
                    Text{
                        anchors.leftMargin: 6;
                        color:"#333333";
                        anchors.verticalCenter: parent.verticalCenter;
                        text: qsTr("  正文");
                        font{ family: localFont.name; pointSize: 11; bold: true; }
                    }
                }
                Rectangle{width: parent.width+4; height: 1; color:"#e8e2e2";}
                Row{
                    width: parent.width - 6; height: rectangle3.height - historyText.height;clip: true;
                    anchors.left: parent.left; anchors.leftMargin: 5;
                    Rectangle
                    {
                        width:parent.width-4;height: parent.height/2 ;
                        color:"#ffffff";
                        anchors.leftMargin: 10;
//                        border.color: Qt.rgba(0,0,0,0.5);
                        Item{width: 2; height: parent.height}
                        Flickable {
                            id: flick
                            width: parent.width-20; height: parent.height-20;
                            anchors {left: parent.left; leftMargin: 10;right: parent.right;rightMargin: 11;top:parent.top;topMargin: 10;}
                            contentWidth: flick.width
                            contentHeight: flick.height
                            clip: true

                            function ensureVisible(r)
                            {
                                if (contentX >= r.x)
                                    contentX = r.x;
                                else if (contentX+width <= r.x+r.width)
                                    contentX = r.x+r.width-width;
                                if (contentY >= r.y)
                                    contentY = r.y;
                                else if (contentY+height <= r.y+r.height)
                                    contentY = r.y+r.height-height;
                            }
                            TextEdit {
                                id:detailsText;
                                width: flick.width
                                height: flick.height
                                focus: true
                                wrapMode: TextEdit.Wrap
                                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle);
                                selectByMouse: true;
                                clip: true;
                                color: "black"; text: "";

                                font{ family: localFont.name; pointSize: 10; }
                            }
                        }
                    }
                }
            }
        }
        Item{
            width: parent.width; height: 30;
            PushButtons{
                 width: 86; height: 23;
                 anchors.right: parent.right;
                 label: qsTr("发送");
                 onClicked: {
                      messageManageModel.saveGPRSMessage(idx, detailsText.text);
                      messageManageModel.getGPRSMessageByFilter(1,messageManageModel.pageSize,idx);
                      detailsText.text = "";
                 }
            }
        }
    }
    Connections{target: messageManageModel;
        onSendAllUsersToHistoryWidget:{
            idx = m_userId;
            searchTextInput.text = m_name;
            phoneTextInput.text = m_telnum;
            messageManageModel.getGPRSMessageByFilter(messageManageModel.pageNo,messageManageModel.pageSize,idx);
        }
        onSendHistoryInfo:{
            tidalTableModel.append( {   "item0": userid,  //用户ID
                                        "item1": message,   //用户名
                                        "item2": sendTime   //发送时间
                                   }
                                   );
        }
    }
}
