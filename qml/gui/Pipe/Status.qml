import QtQuick 2.12
import "../Basic"
import Pipeline 1.0

Status0{
    property string name

    function updateStatus(aInput){
        statuslist.clear()
        for (var i in aInput)
            statuslist.append({cap: aInput[i]})
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            updateStatus(aInput.data())
            aInput.out()
        }, {name: name + "_updateStatus"})
    }
}
