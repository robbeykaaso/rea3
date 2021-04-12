import QtQuick 2.12
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Row {
    property int interval: 4
    property double ratio: 0.3
    property alias caption: caption
    property alias slider: slider
    signal indexChanged(int aIndex)

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

    Slider{
        id: slider
        property int last_idx
        width: parent.width * (1 - ratio - 0.1)
        height: parent.height * 0.8
        anchors.verticalCenter: parent.verticalCenter
        minimumValue: 0
        maximumValue: 100
        value: 0

        function updateInterval(){
            var idx = 0
            if (interval >= 1){
                var step = 100 / interval
                idx = Math.round(value / step)
                value = idx * step
            }
            if (last_idx != idx)
                indexChanged(idx)
            last_idx = idx
        }

        onValueChanged: {
            updateInterval()
        }
        style: SliderStyle{
            handle: Rectangle
            {
                implicitWidth: 8
                implicitHeight:12
                color:"#1f9b98"
                //radius:8
            }
        }
    }
}
