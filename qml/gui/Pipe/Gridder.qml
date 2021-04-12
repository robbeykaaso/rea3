import QtQuick 2.12
import "../Basic"
import Pipeline 1.0

Gridder0{
    Component.onCompleted: {
        Pipeline.add(function(aInput){
            updateViewCount(aInput.data()["size"])
            aInput.out()
        }, {name: name + "_updateViewCount"})
    }
}
