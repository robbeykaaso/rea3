import QtQuick 2.0
import "../Basic"
import Pipeline 1.0

Navigation0 {
    property string name
    onUpdated: {
        Pipeline.run(name + "_updateNavigation", menu, "manual")
    }
    Component.onCompleted: {
        Pipeline.add(function(aInput){
            menu = aInput.data()
            aInput.out()
        }, {name: name + "_updateNavigation", type: "Partial", vtype: "array"})
    }
}
