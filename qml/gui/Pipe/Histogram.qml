import QtQuick 2.0
import "../Basic"

Histogram0 {
    property string name
    title: Pipelines().tr("Score")
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            drawHistomgram(aInput.data()["histogram"])
            aInput.out()
        }, {name: name + "_updateHistogramGUI"})
    }
}
