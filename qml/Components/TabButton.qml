import QtQuick 1.1

Item{
    id: tabButton;
    width: tabListView.count*columnWidth; height: 24;
    property variant elements: ["全部", "船名","MMSI","IMO","船舶类型","船长"];
    property alias indexs: tabListView.currentIndex;
    property int columnWidth: 69;
    property bool isVisible: true;
    signal currentIndexChange( int index );

    ListView {
        id: tabListView;
        anchors.fill: parent;
        model: tabButton.elements;
        delegate: tabListDelegate;
        clip: true; spacing: -1;
        orientation: ListView.Horizontal;
        interactive: false;
        /*-------------------------------------------
         * 代理
         ---------------------------------------------*/
        Component {
            id: tabListDelegate;
            Rectangle{
                width: columnWidth; height: 26; color: "#14487b"; clip: true;
                Rectangle{
                    anchors.fill: parent; clip: true; anchors.margins: 1;
                    Rectangle{ width: parent.width; height: 1; color: "#AFAFAF"; anchors.top: parent.top; }
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: tabListView.currentIndex == index ?  "#ff9123" : "#2b7ece" }
                        GradientStop { position: 0.5; color: tabListView.currentIndex == index ?  "#ff9123" : "#1d5f9f" }
                        GradientStop { position: 1.0; color: tabListView.currentIndex == index ?  "#ff9123" : "#13487b" }
                    }
                    Text {
                        anchors.centerIn: parent;
                        text: elements[index]; color: tabListView.currentIndex == index ? "#0A0A0A" :"#CFCFCF";
                        font{ family: localFont.name; pointSize: 10; bold: true; }
                    }
                    MouseArea { anchors.fill: parent; onClicked: { if(isVisible) tabListView.currentIndex = index; } }
                }
            }
        }
        onCurrentIndexChanged: tabButton.currentIndexChange(tabListView.currentIndex);
    }
}
