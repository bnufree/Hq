import QtQuick 1.1
import "Components"
Rectangle {
    width: 500
    height: 900
    FontLoader { id: localFont; source: "file:fonts/msyhbd.ttf" }

    Column{
       anchors.fill: parent
       Item{width: parent.width; height: 15;}
       Row{
          width: parent.width; height: parent.height - 60;spacing: 0
          Item{width: 8; height: parent.height;}
          Rectangle{
              width: parent.width - 20; height: parent.height;
              color:"#ffffff";
              border{ width: -1; color: "#cccccc"; }
              Column{
                  anchors.fill: parent
                  Item {
                      width: parent.width;  height: 28;clip:true
                      Text {
                          width: 480; height:20; clip: true;
                          color: "#333333"; text: qsTr(" AIS目标（MMSI号）        雷达目标（航迹唯一号）        状态");
                          font{ family: localFont.name; pointSize: 10; }
                      }
                  }
                  Flickable{
                      width: parent.width-6; height: parent.height - 30;  contentHeight: parent.height - 30;clip: true
                      Rectangle{
                          anchors.fill: parent;clip: true  ;  anchors.leftMargin: 0;
                          ListView {
                              id:trackView
                              width: parent.width; height: parent.height
                              model: trackNumModel
                              delegate: trackDelegate
                              focus: true
                              Component {
                                  id: trackDelegate
                                  Rectangle {
                                      id: wrapper2
                                      width: trackView.width
                                      height: 28
                                      color: ListView.isCurrentItem ? "#8edcf3" : "#FFFFFF";
                                      MouseArea {
                                          anchors.fill: parent;
                                          onClicked: { trackView.currentIndex = index;
                                              RadarFusionSplitModel.setMmsisIndex(index);
                                          }
                                      }
                                      Row{
                                          width: parent.width; height: 28;
                                          Text {
                                              text: name;
                                              width: parent.width - 16;
                                              height: 28
                                              anchors.verticalCenter: parent.verticalCente
                                              font{ family: localFont.name; pointSize: 9; }
                                              color: "#202020";
                                          }
                                          Item{
                                              width: 15; height: 15;
                                              BorderImage {
                                                  anchors.centerIn: parent;
                                                  horizontalTileMode: BorderImage.Stretch;
                                                  verticalTileMode: BorderImage.Stretch;
                                                  width: 12; height: parent.height; smooth: true;
                                                  source: "qrc:/ecdisviewer/qml/images/closebutton.png"
                                              }
                                              MouseArea{
                                                  anchors.fill: parent;
                                                  onClicked: {
                                                        trackNumModel.remove(index);
                                                        RadarFusionSplitModel.deleteMmsis(index);
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
          }
       }
       Item{width: parent.width; height: 20;}
       Item{
           width: parent.width;height: 32;
           Row{
               width: 200;anchors.right: parent.right;spacing: 4;
               PushButtons {
                   width: 90;  height: 23;
                   label: qsTr("融合/拆分")
                   onClicked: {RadarFusionSplitModel.resolutionData();}
               }
               PushButtons {
                   width: 90;  height: 23;
                   label: qsTr("取消")
                   onClicked: { RadarFusionSplitModel.closeDialog();}
               }
           }
       }
       ListModel {id:mmsiModel}
       ListModel {id:trackNumModel}
       Connections{target: RadarFusionSplitModel;
           onSendClearMmsi:{mmsiModel.clear();}
           onSendClearTrack:{trackNumModel.clear();}
            onSendMmsiData:{
                mmsiModel.append({"index_m": index, "name":name_m});
            }
            onSendTrackData:{
                trackNumModel.append({"index_m": index, "name":name_m});
            }
       }
       Component.onCompleted: RadarFusionSplitModel.updateData();
    }
}
