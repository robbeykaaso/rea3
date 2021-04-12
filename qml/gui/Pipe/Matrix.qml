import QtQuick 2.0
import "../Basic"
import Pipeline 1.0

Matrix0 {
    property string name
    onSelected: function(aIndex){
        Pipeline.run(name + "_matrixSelected", aIndex, "manual")
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            aInput.out()
        }, {name: name + "_matrixSelected", type: "Partial", vtype: "number"})

        Pipeline.add(function(aInput){
            updateModel(aInput.data())
            aInput.out()
        }, {name: name + "_updateMatrix"})
    }
}
