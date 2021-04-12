import QtQuick 2.12
import QtQuick.Layouts 1.12

Rectangle {
    property var customfont: {"size": 14}

    property var menu: ['1', '2', '3']
    signal updated()

    RowLayout {
        spacing: 1
        height: parent.height

        Repeater {
            model: menu
            Row {
                height: parent.height
                leftPadding: 5
                focus: true
                id: _row
                Text {
                    id: control
                    color: '#000'
                    height: 30
                    font.pixelSize: customfont.size
                    verticalAlignment: Text.AlignVCenter
                    text: modelData

                    MouseArea {
                        anchors.fill : parent
                        enabled: index < menu.length - 1
                        hoverEnabled: true
                        onEntered: {
                            control.color = 'blue'
                            control.font.underline = true
                            cursorShape = Qt.PointingHandCursor
                        }
                        onHoveredChanged: {
                            control.color = '#000'
                            control.font.underline = false
                            cursorShape = Qt.ArrowCursor
                        }

                        onClicked:  {
                            menu.splice(index+1, menu.length - index - 1)
                            updated()
                        }
                    }
                }

                Text {
                    id: text
                    height: 30
                    width: 15
                    text: index < menu.length - 1 ? ">" : ''
                    font.pixelSize: customfont.size
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}
