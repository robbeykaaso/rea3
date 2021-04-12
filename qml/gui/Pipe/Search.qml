import QtQuick 2.0
import "../Basic"
import Pipeline 1.0

Search0 {
    property string name
    onAccepted: {
        Pipeline.run(name + "_Searched", "", "manual")
    }
    Component.onCompleted: {
        Pipeline.add(function(aInput){
            focus = false
            aInput.setData(text).out()
        }, {name: name + "_Searched", type: "Partial", vtype: "string"})
    }
}
