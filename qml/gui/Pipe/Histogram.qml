import QtQuick 2.0
import Pipeline 1.0
import "../Basic"

Histogram0 {
    property string name
    title: Pipeline.tr("Score")
    Component.onCompleted: {
        Pipeline.add(function(aInput){
            drawHistomgram(aInput.data()["histogram"])
            aInput.out()
        }, {name: name + "_updateHistogramGUI"})
    }
}
