import QtQuick 1.1
import "Components"
Rectangle {
    width: 600; height: 500; color: "#ffffff";
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }
    MouseArea{ anchors.fill: parent; }
    property bool aisFlag: false;
    property bool messageFlag: false;
    property bool typereorder: true;
    property bool addInfo: true;
    signal visibleFlag(bool m_flag);
    signal addFlag(bool m_add);
    Rectangle {
        anchors.fill: parent; color: "#fefefe";
        border{ width: 1; color: "#e8e2e2"; }
        Row{
            anchors{ fill: parent; margins: 4; } spacing: 6; clip:true;
            Rectangle{
                id:leftRectangle
                height: parent.height -4; width: parent.width/3; clip:true;
                border{ width: 1; color: "#e8e2e2"; }
                color:"#ffffff"
                Column{
                    width:parent.width; height: parent.height;
                    anchors.margins: 2; spacing:1; clip:true;
                    Item{width: parent.width; height: 2;}
                    Rectangle{
                        id:aisTab
                        width: parent.width- 6; height: 25; anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.left: parent.left;
                        anchors.leftMargin: 3;
                        color:"#457dac";
                        //                        Rectangle { width: parent.width; height: parent.height*0.5; color: "#457dac" }
                        //                        Rectangle { width: parent.width; height: parent.height*0.5; color: "#424d8f"; anchors.bottom: parent.bottom }

                        Text {
                            anchors.verticalCenter: parent.verticalCenter;
                            color: "#ffffff"; text: qsTr("AIS消息");
                            font{ family: localFont.name; pointSize: 11; }
                        }
                    }

                    ListView{
                        id:aisViw
                        width: parent.width; height: 100;
                        visible: aisFlag

                    }
                    Row{
                        width: parent.width; height: 25;spacing: 1;
                        Item{width: 1; height: parent.height;}
                        Rectangle{
                            id:messageTab
                            width: parent.width - 6 -50; height: 25;
                            anchors.leftMargin: 3;
                            color:"#457dac";
                            //                        Rectangle { width: parent.width; height: parent.height*0.5; color: "#457dac" }
                            //                        Rectangle { width: parent.width; height: parent.height*0.5; color: "#424d8f"; anchors.bottom: parent.bottom }
                            Row{
                                width: parent.width; height: 25;
                                Item{width: 10; height: 25;}
                                BorderImage {
                                    id: table_arrow;
                                    width: 8; height: 8;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    horizontalTileMode: BorderImage.Stretch;
                                    verticalTileMode: BorderImage.Stretch;
                                    source: "qrc:/ecdisviewer/qml/images/arrow-right-black.png";
                                    smooth: true;
                                }
                                Item{width: 10; height: parent.height;}
                                Text {
                                    anchors.verticalCenter: parent.verticalCenter;
                                    color: "#ffffff"; text: qsTr(" 短消息");
                                    font{ family: localFont.name; pointSize: 11; }
                                }
                            }
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    messageFlag = !messageFlag;
                                    visibleFlag(messageFlag);
                                    typereorder ? typereorder = false : typereorder = true;
                                    if(!typereorder)
                                    {
                                        table_arrow.source = "qrc:/ecdisviewer/qml/images/arrow-right-orange.png";
                                        table_arrow.rotation = 90;
                                    }
                                    else{
                                        table_arrow.source = "qrc:/ecdisviewer/qml/images/arrow-right-black.png";
                                        table_arrow.rotation = 0;
                                    }
                                }
                            }
                        }
                        Item{
                            width: 25; height: 25;
                            PushButtons{
                                width: 25; height: 25;
                                anchors.right: parent.right;
                                backgroundColor:"#457dac";
                                label: qsTr("-");
                                onClicked: {
                                    if(showmessage.visible && messageRectangle.visible)
                                    {
                                        if(messageViw.count >= 0 && usermessageModel.count >0)
                                        messageManageModel.delMessageUserById(usermessageModel.get(messageViw.currentIndex).id_x);
                                    }
                                }
                            }
                        }
                        Item{
                            width: 25; height: 25;
                            PushButtons{
                                width: 25; height: 25;
                                anchors.right: parent.right;
                                backgroundColor:"#457dac";
                                label: qsTr("+");
                                onClicked: {
                                    addInfo = !addInfo;
                                    addFlag(addInfo);
                                }
                            }
                        }
                    }
                    Item{width: parent.width; height: 2;}
                    Item{
                        id:showmessage
                        width: parent.width; height: 325
                        visible: addInfo
                    /*-------------------------------------------
                    * 搜索框
                    ---------------------------------------------*/
                    Rectangle{
                        id:searchRectangle
                        width: parent.width -6; height: 25;
                        anchors.left: parent.left;
                        anchors.leftMargin: 3;
                        color: "#ffffff";
                        visible: addInfo;
                        border{ width: 1; color: "#abd1ed"; }
                        TextInput {
                            id: searchInput;
                            anchors{ fill: parent; margins: 3; verticalCenter: parent.verticalCenter; }
                            font{ family: localFont.name; pointSize: 9; bold: false;}
                            focus: true; color: "#333333";
                            Keys.onReturnPressed: messageManageModel.getMessageAllUsers(searchInput.text );
                            Keys.onEnterPressed: messageManageModel.getMessageAllUsers(searchInput.text );
                        }
                        Text {
                            id: typeSomething
                            anchors{ fill: parent; margins: 3; verticalCenter: parent.verticalCenter; }
                            font{ family: localFont.name; pointSize: 8; italic: true;}
                            text: " 搜索联系人";
                            color: "#878080";
                        }
                        BorderImage {
                            id: searchBtn;
                            anchors{ verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 12; }
                            width: 13; height: 14; smooth: true;
                            border { left: 3; top: 3; right: 3; bottom: 3 }
                            horizontalTileMode: BorderImage.Repeat;
                            verticalTileMode: BorderImage.Repeat;
                            source: "qrc:/core/qml/image/tab_search.png";
                            MouseArea {  anchors.fill: parent; }
                        }
                        states: State {
                            name: "hasText"; when: searchInput.text != ''
                            PropertyChanges { target: typeSomething; opacity: 0 }
                            PropertyChanges { target: searchInput; opacity: 1 }
                        }
                        transitions: [
                            Transition { from: ""; to: "hasText"; NumberAnimation { exclude: typeSomething; properties: "opacity" } },
                            Transition { from: "hasText"; to: ""; NumberAnimation { properties: "opacity" } }
                        ]
                    }

                    Rectangle{
                        id:messageRectangle
                        width: parent.width - 6; height: 300;clip:true;
                        anchors.left: parent.left;
                        anchors.leftMargin: 3;
                        anchors.top: searchRectangle.bottom;
                        anchors.topMargin: 5;
                        visible: messageFlag;
                        ListView{
                            id:messageViw
                            width: parent.width; height: 300;
                            delegate:messageDelegate;
                            model:/*messageModel*/usermessageModel
                            currentIndex:0;
//                            ListModel{
//                                id: messageModel
//                                ListElement {idx:0; names: "  张三"; }
//                                ListElement {idx:1; names: "  李四"; }
//                                ListElement {idx:2; names: "  王五"; }
//                                ListElement {idx:3; names: "  小魏"; }
//                            }

                            Component{
                                id:messageDelegate
                                Rectangle{
                                    width: messageViw.width-1; height: 28;
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: messageViw.currentIndex === index ? "#b0efff": "#ffffff"; }
                                        GradientStop { position: 1.0; color: messageViw.currentIndex === index ? "#b0efff": "#ffffff"; }
                                    }
                                    Text{
                                        anchors{left: parent.left;  leftMargin: 5; top:parent.top; topMargin: 3;}
                                        width: parent.width; height:28;
                                        font{family: localFont.name; pointSize: 9;}
                                        color:"#333333";
                                        smooth:true;
                                        text:m_name;
                                    }
                                    MouseArea{
                                        anchors.fill: parent;
                                        onClicked: {
                                            messageViw.currentIndex = index;
                                        }
                                        onDoubleClicked: {
                                            if(messageViw.count >= 0 && usermessageModel.count >0)
                                            messageManageModel.sendAllUsersToHistoryWidget(usermessageModel.get(messageViw.currentIndex).id_x,
                                                                                           usermessageModel.get(messageViw.currentIndex).m_name,
                                                                                           usermessageModel.get(messageViw.currentIndex).mm_telnum
                                                                                           );
                                        }
                                    }
                                }

                            }
                        }
                    }
                    }
                    Item{
                        id:addmessage
                        width: parent.width; height: 325
                        visible: !addInfo;
                        Column{
                            width: parent.width;
                            height: 325;
                            Row{
                                width: parent.width;
                                height: 28;
                                Text {
                                    id:nameText
                                    width: 50; height: 26;
                                    color: "#333333";
                                    anchors.verticalCenter: parent.verticalCenter;
                                    text: qsTr("  姓名");
                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                }
                                LineEditInput{
                                    id: nameInput;
                                    width: 150; height: 24;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    onTextChanged: {
                                    }
                                }
                            }
                            Row{
                                width: parent.width;
                                height: 28;
                                Text {
                                    id:searchText
                                    width: 50; height: 26;
                                    color: "#333333";
                                    anchors.verticalCenter: parent.verticalCenter;
                                    text: qsTr("  电话");
                                    font{ family: localFont.name; pointSize: 10; bold: false; }
                                }
                                LineEditInput{
                                    id: searchTextInput;
                                    width: 150; height: 24;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    onTextChanged: {
                                    }
                                }
                            }

                            Row{
                                width: 100;
                                height: 28;
                                spacing: 4;
                                Item{width: 60; height: parent.height;}
                                PushButtons{
                                    width: 70; height: 25;
                                    label: qsTr("保存");
                                    onClicked: {
                                        if(!isPhone(searchTextInput.text))
                                        {
                                            messageManageModel.messageBox("电话格式错误。");
                                            return;
                                        }
                                        addInfo = !addInfo;
                                        addFlag(addInfo);
                                        messageManageModel.saveMessageUser("0", nameInput.text, searchTextInput.text);
                                    }
                                }
                                PushButtons{
                                    width: 70; height: 25;
                                    label: qsTr("取消");
                                    onClicked: {
                                        addInfo = !addInfo;
                                        addFlag(addInfo);

                                        nameInput.text = "";
                                        searchTextInput.text = "";
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle{
                id:rightRectangle
                height: parent.height; width:  parent.width - leftRectangle.width -8; clip:true;
                color:"#ffffff";
                ListView{
                    id:centersView
                    anchors.fill: parent;
                    model:itemModel;
                    currentIndex:0;
                    clip:true;
                    highlightMoveSpeed: 9999999;
                    preferredHighlightBegin: 0;
                    preferredHighlightEnd: 0
                    highlightRangeMode: ListView.StrictlyEnforceRange
                    orientation: ListView.Vertical;
                    snapMode: ListView.SnapOneItem;
                    flickDeceleration: 400;
                    interactive: false;
                    VisualItemModel {
                        id: itemModel;
                        MessageInfo{
                            width: centersView.width; height: centersView.height;
                        }
                    }
                }

            }

        }
    }
    Component.onCompleted: messageManageModel.getMessageAllUsers("");
    ListModel{
        id: usermessageModel
    }
    onVisibleFlag: messageRectangle.visible = m_flag;
    onAddFlag: addInfo = m_add;
    Connections{target: messageManageModel;
        onSendClearList:usermessageModel.clear();
        onSendAllUsers:{
            usermessageModel.append({"id_x": m_userId,
                                    "m_name": m_name,
                                    "mm_telnum":m_telnum});
        }

    }
    function isPhone(strPhone)
    {
        if (strPhone.search( /(^[0-9]{3,4}\-[0-9]{7,8}$)|(^[0-9]{7,8}$)|(^\([0-9]{3,4}\)[0-9]{3,8}$)|(^0{0,1}13[0-9]{9}$)|(13\d{9}$)|(15[0135-9]\d{8}$)|(18\d{9}$)/) === -1)
        {
            return false;
        }
        else{
            return true;
        }
    }
}
