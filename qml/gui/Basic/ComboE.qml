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
        EditCombo{
            id: cmb
            anchors.fill: parent
            height: parent.height
            activeColor: "lightskyblue"
        }
    }
}
