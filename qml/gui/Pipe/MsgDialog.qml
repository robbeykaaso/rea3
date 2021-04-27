import QtQuick 2.12
import QtQuick.Dialogs 1.2
import Pipeline 1.0

MessageDialog {
    property string service_tag
    property var service_scope
    property bool selected
    title: "Hello"
    text: "World"

    onAccepted: {
        if (!selected){
            selected = true
            Pipeline.run("messagePoped", true, service_tag, service_scope)
        }
    }

    onRejected: {
        Pipeline.run("messagePoped", false, service_tag, service_scope)
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            var dt = aInput.data()
            selected = false
            title = dt["title"]
            text = dt["text"]
            service_tag = aInput.tag()
            service_scope = aInput.scope()
            open()
        }, {name: "popMessage", type: "Delegate", delegate: "messagePoped"})

        Pipeline.add(function(aInput){
            aInput.out()
        }, {name: "messagePoped", type: "Partial"})
    }
}
