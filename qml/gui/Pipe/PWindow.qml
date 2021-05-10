import QtQuick 2.0
import "../Basic"
import Pipeline 1.0

PWindow0 {
    property string service_tag
    property var service_scope
    tr: Pipeline.tr
    customGetParam: function(aTrig){
        return Pipeline.input("", "", {}, true).asyncCall(aTrig).data()
    }

    onAccept: {
        close()
        Pipeline.run("_paramSet", outputModel(), service_tag, service_scope)
    }

    onReject: {
        close()
        Pipeline.run("_paramSet", {}, service_tag, service_scope)
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            aInput.out()
        }, {name: "_paramSet", type: "Partial"})

        Pipeline.add(function(aInput){
            service_tag = aInput.tag()
            service_scope = aInput.scope()
            showModel(aInput.data())
            aInput.out()
        }, {name: "_setParam", type: "Delegate", delegate: "_paramSet"})
    }
}
