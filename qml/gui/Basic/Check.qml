import QtQuick 2.12
import QtQuick.Controls 2.5

Row {
    property alias caption: caption
    property alias check: chk
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

    CheckBox {
        id: chk
        width: parent.width * (1 - ratio - 0.1)
        height: parent.height * 0.8
        anchors.verticalCenter: parent.verticalCenter
        indicator: Rectangle {
            implicitWidth: parent.height * 0.6
            implicitHeight: parent.height * 0.6
            anchors.verticalCenter: chk.verticalCenter
            anchors.horizontalCenter: chk.horizontalCenter
            radius: width / 4
            color: "white"
            border.color: "gray"

            Rectangle {
                x: (parent.height - width) * 0.5
                y: (parent.height - height) * 0.5
                width: parent.height * 0.55
                height: parent.height * 0.55
                //anchors.verticalCenter: chk.verticalCenter
                //anchors.horizontalCenter: chk.horizontalCenter
                radius: width / 4
                color: "gray"
                visible: parent.parent.checked
            }
        }
    }
}
