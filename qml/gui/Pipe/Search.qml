import QtQuick 2.0
import "../Basic"

Search0 {
    property string name
    onAccepted: {
        Pipelines().run(name + "_Searched", "", "manual")
    }
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            focus = false
            aInput.setData(text).out()
        }, {name: name + "_Searched", type: "Partial"})
    }
}
