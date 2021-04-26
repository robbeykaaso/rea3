import QtQuick 2.12
import QtQuick.Dialogs 1.2
import Pipeline 1.0

ColorDialog{
    property string name
    property string service_tag

    onAccepted: {
        Pipeline.run(name + "_colorSelected", color.toString(), service_tag)
        close()
    }
    onRejected: {
        Pipeline.run(name + "_colorSelected", "", service_tag)
        close()
    }
    Component.onCompleted: {
        Pipeline.add(function(aInput){
            aInput.out()
        }, {name: name + "_colorSelected", type: "Partial"})

        Pipeline.add(function(aInput){
            service_tag = aInput.tag()
            open()
        }, {name: name + "_selectColor", type: "Delegate", delegate: name + "_colorSelected"})
    }
}
