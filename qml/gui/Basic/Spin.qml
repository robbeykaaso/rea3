import QtQuick 2.9
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Row {
    property alias caption: caption
    property alias background: bak
    property alias spin: val
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
        SpinBox {
            id: val
            style: SpinBoxStyle {
                background: Rectangle {
                    //radius: 6
                }
            }
            anchors.fill: parent
            font.pixelSize: 12
            minimumValue: 0
            maximumValue: 200
        }
    }
}
