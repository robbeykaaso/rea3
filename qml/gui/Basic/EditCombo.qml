import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Row {
    id: root

    property var modellist: []
    property string currentSelect: ''
    property string activeColor: 'Silver'

    property int currentIndex: -1
    property bool popupOpen: false

    function getCurrentSelect() {
        return currentSelect
    }

    TextField {
        id: inputText
        text: currentSelect
        selectByMouse: true
        width: root.width - 10
        height: root.height
        font.pixelSize: 12
        background: Rectangle {
            implicitWidth: root.width - 14
            implicitHeight: root.height - 2
            border.width: 0
            y: 1
            x: 1
        }
        verticalAlignment: Text.AlignVCenter
        onTextChanged: {
            currentSelect = inputText.text
        }
    }

    Image {
        id: openOrClose
        width: 10
        height: root.height
        anchors.verticalCenter: parent.verticalCenter
        transform: Rotation {
            origin.x: 5
            origin.y: height / 2
            angle: popupOpen ? 180 : 0
        }

        source: "../../resource/triangle-down.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                popupOpen ? popup.close() : (modellist.length > 0
                                             && popup.open())
                popupOpen = !popupOpen
            }
        }
    }

    Popup {
        id: popup
        width: root.width
        y: root.height
        onVisibleChanged: {
            if (popupOpen) {
                popupOpen = !popupOpen
            }
        }

        ListView {
            id: listview
            width: root.width
            x: -11
            y: -11

            delegate: Item {
                id: item
                width: root.width - 2
                height: root.height - 1
                Rectangle {
                    width: root.width - 2
                    color: currentIndex === index ? activeColor : 'white'
                    height: parent.height
                    Text {
                        text: result
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        x: 10
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: currentIndex = index
                        onClicked: {
                            currentSelect = result
                            popup.close()
                            popupOpen = false
                        }
                    }
                }
            }
            model: ListModel {
            }
        }
    }

    function updateGUI(){
        listview.model.clear()
        popup.height = modellist.length * root.height - 1
        listview.height = modellist.length * (root.height - 1)
        for (var i = 0; i < modellist.length; ++i) {
            listview.model.append({
                                      "result": modellist[i]
                                  })
            if (currentSelect === modellist[i]) {
                currentIndex = i
            }
        }
    }

    Component.onCompleted: {
        updateGUI()
    }
}
