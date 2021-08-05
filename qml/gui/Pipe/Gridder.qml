import QtQuick 2.12
import "../Basic"

Gridder0{
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            updateViewCount(aInput.data())
            aInput.out()
        }, {name: name + "_updateViewCount"})
    }
}
