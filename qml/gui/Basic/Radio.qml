import QtQuick 2.12
import QtQuick.Controls 2.5

RadioButton {
    id: root
    width: 120
    height: 30
    text: qsTr("hello")

    indicator: Rectangle {
        implicitWidth: parent.height * 0.6
        implicitHeight: parent.height * 0.6
        anchors.verticalCenter: parent.verticalCenter

        radius: width / 2
        color: "white"
        border.width: root.visualFocus ? 2 : 1
        border.color: "gray"

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width * 0.7
            height: parent.width * 0.7
            radius: width / 2
            color: "gray"
            visible: root.checked
        }
    }
    contentItem: Text {
        text: parent.text
        font.pixelSize: 12
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
