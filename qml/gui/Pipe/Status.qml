import QtQuick 2.12
import "../Basic"

Status0{
    property string name

    function updateStatus(aInput){
        statuslist.clear()
        for (var i in aInput)
            statuslist.append({cap: aInput[i]})
    }

    Component.onCompleted: {
        Pipelines().add(function(aInput){
            updateStatus(aInput.data())
            aInput.out()
        }, {name: name + "_updateStatus"})
    }
}
