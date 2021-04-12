import QtQuick 2.12
import QtQuick.Controls 2.5

FocusScope{
    id: fs
    property string page_text
    property string page_suffix
    property string page_title: "Page:"
    property string background_color: "lightskyblue"
    property int fontsize: 12
    signal selectSpecificPage(var aIndex)
    signal selectNeighbourPage(var aNext)
    anchors.horizontalCenter: parent.horizontalCenter
    width: 130
    height: 30
    Row{
        anchors.fill: parent
        Edit{
            caption.text: page_title
            input.text: fs.page_text
            input.font.pixelSize: fontsize
            background.color: background_color
            width: 60
            ratio: 0.5
            input.onAccepted: {
                selectSpecificPage(parseInt(input.text))
                fs.focus = false
            }
        }
        Label{
            text: fs.page_suffix
            font.pixelSize: fontsize
            color: "black"
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            background: Rectangle{
                color: "transparent"
                border.color: "gray"
                radius: 3
            }
            anchors.verticalCenter: parent.verticalCenter
            height: 20
            width: 20
            font.pixelSize: fontsize
            text: "<"
            onClicked: selectNeighbourPage(false)
        }
        Button{
            background: Rectangle{
                color: "transparent"
                border.color: "gray"
                radius: 3
            }
            anchors.verticalCenter: parent.verticalCenter
            height: 20
            width: 20
            font.pixelSize: fontsize
            text: ">"
            onClicked: selectNeighbourPage(true)
        }
    }
}
