﻿import QtQuick 2.0
import "../Basic"

PageList0 {
    property string name: ""
    tr: Pipelines().tr
    onSelected: {
        Pipelines().run(name + "_listViewSelected", [], "manual")
    }
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            aInput.setData(selects).out()
        }, {name: name + "_listViewSelected", type: "Partial"})

        Pipelines().add(function(aInput){
            updateModel(aInput.data())
            aInput.out()
        }, {name: name + "_updateListView"})
    }
}
