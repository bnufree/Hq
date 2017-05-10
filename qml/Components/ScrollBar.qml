// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: scrollBar
    signal scaleXChanged(real pos)
    signal scaleYChanged(real pos)

    // The properties that define the scrollbar's state.
    // position and pageSize are in the range 0.0 - 1.0.  They are relative to the
    // height of the page, i.e. a pageSize of 0.5 means that you can see 50%
    // of the height of the view.
    // orientation can be either Qt.Vertical or Qt.Horizontal
    property real position
    property real pageSize
    property variant orientation : Qt.Vertical
    clip: true

    // A light, semi-transparent background
    Rectangle {
        id: background
        anchors.fill: parent
        radius: orientation == Qt.Vertical ? (width/2 - 1) : (height/2 - 1)
        color: "white"
        opacity: 0.3
    }

    // Size the bar to the required size, depending upon the orientation.
    Rectangle {
        x: orientation == Qt.Vertical ? 1 : (scrollBar.position * (scrollBar.width-2) + 1)
        y: orientation == Qt.Vertical ? (scrollBar.position * (scrollBar.height-2) + 1) : 1
        width: orientation == Qt.Vertical ? (parent.width-2) : (scrollBar.pageSize * (scrollBar.width-2))
        height: orientation == Qt.Vertical ? (scrollBar.pageSize * (scrollBar.height-2)) : (parent.height-2)
        radius: orientation == Qt.Vertical ? (width/2 - 1) : (height/2 - 1)
        color: "#d0d0d0"
        opacity: 0.7

        /*
        * 发送滚动条被拖动时的比例
        */
        MouseArea {
            anchors.fill: parent

            drag.target: parent
            drag.axis: orientation == Qt.Horizontal? Drag.XAxis : Drag.YAxis
            drag.maximumX: background.width - parent.width
            drag.minimumX: 0
            drag.maximumY: background.height - parent.height
            drag.minimumY: 0

            /*onMousePositionChanged*/
            onPositionChanged: {
                //console.log("back width:"+background.width + " parent width:" + parent.width + " x:" + parent.x)
                var scale = orientation == Qt.Horizontal? parent.x/(background.width - parent.width) : parent.y/(background.height - parent.height)
                if(orientation == Qt.Horizontal)
                {
                    scrollBar.scaleXChanged( scale )
                } else
                {
                    scrollBar.scaleYChanged( scale )
                }

                //console.log("scrollbar changed as scale:" + scale)

            }
        }

    }
}
