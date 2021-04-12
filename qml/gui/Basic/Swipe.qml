import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    property alias tabs: tabbar
    property alias pages: swipeView
    width: 300
    height: 500
    TabBar {
        id: tabbar
        width: parent.width
        height: 30

        TabButton {
            text: qsTr("PageOne")
            font.pixelSize: 12
        }
        TabButton {
            text: qsTr("PageTwo")
            font.pixelSize: 12
        }
        TabButton {
            text: qsTr("PageTree")
            font.pixelSize: 12
        }
        TabButton {
            text: qsTr("PageFour")
            font.pixelSize: 12
        }
    }
    SwipeView {  //stackLayout
        id: swipeView
        y: 30
        width: parent.width
        height: parent.height - 30
        currentIndex: tabbar.currentIndex

        Item {
            Label {
                text: qsTr("page one")
                anchors.centerIn: parent
                font.pixelSize: 30
                font.italic: true
                color: "yellow"
            }
        }
        Item {
            Label {
                text: qsTr("page two")
                anchors.centerIn: parent
                font.pixelSize: 30
                font.italic: true
                color: "yellow"
            }
        }
        Item {
            Label {
                text: qsTr("page three")
                anchors.centerIn: parent
                font.pixelSize: 30
                font.italic: true
                color: "yellow"
            }
        }
        Item {
            Label {
                text: qsTr("page four")
                anchors.centerIn: parent
                font.pixelSize: 30
                font.italic: true
                color: "yellow"
            }
        }
    }
    PageIndicator {
        id: indicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
