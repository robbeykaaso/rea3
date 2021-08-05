import QtQuick 2.0
import "../Basic"

PWindow0 {
    property string service_tag
    property var service_scope
    tr: Pipelines().tr
    customGetParam: function(aTrig){
        return Pipelines().input("", "", {}, true).asyncCall(aTrig).data()
    }

    onAccept: {
        close()
        Pipelines().run("_paramSet", outputModel(), service_tag, service_scope)
    }

    onReject: {
        close()
        Pipelines().run("_paramSet", {}, service_tag, service_scope)
    }

    Component.onCompleted: {
        Pipelines().add(function(aInput){
            aInput.out()
        }, {name: "_paramSet", type: "Partial"})

        Pipelines().add(function(aInput){
            service_tag = aInput.tag()
            service_scope = aInput.scope()
            showModel(aInput.data())
            aInput.out()
        }, {name: "_setParam", type: "Delegate", delegate: "_paramSet"})
    }
}
