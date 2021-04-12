import QtQuick 2.12
import QtQuick.Controls 2.5

Column {
    id: column
    property alias titlebar: row
    property alias loglist: loglist
    property alias typelist: typelist
    property alias closepanel: closepanel
    property alias loglevel: loglevel

    property alias logmenu: logmenu
    property alias clear: clear
    height: 180
    width: 540
    Row {
        id: row
        width: parent.width
        height: parent.height * 0.3
        Row {
            width: parent.width * 0.8 - parent.height
            height: parent.height
            spacing: -1

            Repeater {
                id: typelist
                model: ListModel {
                    ListElement {
                        cap: "system"
                        clr: "transparent"
                    }
                    ListElement {
                        cap: "train"
                        clr: "transparent"
                    }
                }

                delegate: Button {
                    width: parent.width * 0.2
                    height: parent.height
                    text: cap
                    background: Rectangle {
                        border.color: "gray"
                        color: clr
                    }
                }
            }
        }
        Rectangle {
            height: parent.height
            width: parent.width * 0.2
            color: "transparent"
            border.color: "gray"
            ComboBox {
                id: loglevel
                font.weight: Font.ExtraLight
                font.family: "Courier"
                anchors.fill: parent
                currentIndex: 0
                model: [qsTr("Opt1"), qsTr("Opt2"), qsTr("Opt3")]
                font.pixelSize: 13
            }
        }
        Button {
            id: closepanel
            width: parent.height
            height: parent.height
            text: "X"
            background: Rectangle {
                border.color: "gray"
                color: "transparent"
            }
        }
    }

    ListView {
        width: parent.width * 0.98
        height: parent.height * 0.7
        x: parent.width * 0.02
        spacing: 2
        topMargin: spacing
        clip: true
        model: ListModel {
            id: loglist
        }

        delegate: Text {
            text: msg
            color: "white"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            width: parent.width
        }
        ScrollBar.vertical: ScrollBar {
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            Menu {
                id: logmenu
                MenuItem {
                    id: clear
                    text: qsTr("Clear")
                }
            }
        }
    }
}
