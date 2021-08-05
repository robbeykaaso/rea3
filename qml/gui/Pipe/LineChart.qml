import QtQuick 2.0
import "../Basic"

LineChart0 {
    property string name
    title: Pipelines().tr("Demo")

    Component.onCompleted: {
        Pipelines().add(function(aInput){
            updateModel(aInput.data())
            aInput.out()
        }, {name: name + "_updateLineChart"})
    }
}
