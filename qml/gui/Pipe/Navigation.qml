import QtQuick 2.0
import "../Basic"

Navigation0 {
    property string name
    onUpdated: {
        Pipelines().run(name + "_updateNavigation", menu, "manual")
    }
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            menu = aInput.data()
            aInput.out()
        }, {name: name + "_updateNavigation", type: "Partial"})
    }
}
