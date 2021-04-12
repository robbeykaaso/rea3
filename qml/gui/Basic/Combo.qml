import QtQuick 2.12
import QtQuick.Controls 2.5

Row {
    property alias caption: caption
    property alias background: bak
    property alias combo: cmb
    property double ratio: 0.3
    width: 120
    height: 30

    spacing: width * 0.05
    Text {
        id: caption
        text: "hello:"
        //horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
        width: parent.width * ratio
        height: parent.height * 0.8
    }

    Rectangle {
        id: bak
        color: "white"
        width: parent.width * (1 - ratio - 0.1)
        height: parent.height * 0.8
        anchors.verticalCenter: parent.verticalCenter
        ComboBox {
            id: cmb
            anchors.fill: parent
            model: ["hello", "world"]
            font.pixelSize: 12
            background: Rectangle {
                border.color: "transparent"
            }

            delegate: ItemDelegate {
                id: itemDlgt
                width: cmb.width
                height: cmb.height
                padding: 0

                contentItem: Text {
                    id: textItem
                    text: modelData
                    color: hovered ? "white" : "gray"
                    font: cmb.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 3
                }

                background: Rectangle {
                    color: itemDlgt.hovered ? "gray" : "white"
                    anchors.left: itemDlgt.left
                    anchors.leftMargin: 0
                }
                //onPressed: console.log("hello")
            }
        }
    }
}
