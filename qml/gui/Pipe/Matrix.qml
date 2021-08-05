import QtQuick 2.0
import "../Basic"

Matrix0 {
    property string name
    onSelected: function(aIndex){
        Pipelines().run(name + "_matrixSelected", aIndex, "manual")
    }

    Component.onCompleted: {
        Pipelines().add(function(aInput){
            aInput.out()
        }, {name: name + "_matrixSelected", type: "Partial"})

        Pipelines().add(function(aInput){
            updateModel(aInput.data())
            aInput.out()
        }, {name: name + "_updateMatrix"})
    }
}
