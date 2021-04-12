import QtQuick 2.12
import QtQuick.Controls 1.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.2

StatusBar {
    property alias statuslist: data
    width: 300
    height: 30
    style: StatusBarStyle {
        background: Rectangle {
            color: "lightskyblue"
        }
    }

    Row {
        anchors.fill: parent
        Repeater {
            model: ListModel {
                id: data
                ListElement {
                    cap: "status1xxxxxxxxxxxx"
                }
                ListElement {
                    cap: "status2"
                }
            }

            delegate: Row {
                height: parent.height

                Label {
                    text: cap
                    anchors.verticalCenter: parent.verticalCenter
                }
                ToolSeparator {
                    //height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
