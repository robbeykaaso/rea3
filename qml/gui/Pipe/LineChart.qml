import QtQuick 2.0
import "../Basic"
import Pipeline 1.0

LineChart0 {
    property string name
    title: Pipeline.tr("Demo")

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            updateModel(aInput.data())
            aInput.out()
        }, {name: name + "_updateLineChart", vtype: "array"})
    }
}
