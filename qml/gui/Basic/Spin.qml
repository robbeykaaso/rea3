import QtQuick 2.9
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import "../Style"

Row {
    property alias caption: caption
    property alias background: bak
    property alias spin: val
    property double ratio: CStyle.spin.ratio
    property QtObject cstyle: SSpin
    width: cstyle.width
    height: cstyle.height

    spacing: cstyle.spacing(this)

    Text {
        id: caption
        text: "hello:"
        //horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: cstyle.text.fontSize
        width: cstyle.text.width(this)
        height: cstyle.text.height(this)
    }

    Rectangle {
        id: bak
        color: "white"
        width: cstyle.rect.width(this)
        height: cstyle.rect.height(this)
        anchors.verticalCenter: parent.verticalCenter
        SpinBox {
            id: val
            style: cstyle.rect.spinBox.style
            anchors.fill: parent
            font.pixelSize: cstyle.rect.spinBox.fontSize
            minimumValue: cstyle.rect.spinBox.min
            maximumValue: cstyle.rect.spinBox.max
        }
    }
}
