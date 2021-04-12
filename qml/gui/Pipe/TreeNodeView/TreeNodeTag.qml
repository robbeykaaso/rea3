import QtQuick 2.12
import QtQuick.Controls 2.5

Button{
    property string comment
    text: "?"
    width: 12
    height: width

    onClicked: {
        detail.open()
    }
    Popup{
        id: detail
        Text{
            text: comment
            font.pixelSize: 12
        }
    }
}
