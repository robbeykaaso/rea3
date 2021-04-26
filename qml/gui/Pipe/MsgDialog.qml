import QtQuick 2.12
import QtQuick.Dialogs 1.2
import Pipeline 1.0

MessageDialog {
    property string service_tag
    property var cache
    title: "Hello"
    text: "World"

    onAccepted: {
        if (!cache["ok"]){
            cache["ok"] = true
            Pipeline.run("messagePoped", cache, service_tag)
        }
    }

    onRejected: {
        Pipeline.run("messagePoped", cache, service_tag)
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            cache = aInput.data()
            cache["ok"] = false
            title = cache["title"]
            text = cache["text"]
            service_tag = aInput.tag()
            open()
        }, {name: "popMessage", type: "Delegate", delegate: "messagePoped"})

        Pipeline.add(function(aInput){
            aInput.out()
        }, {name: "messagePoped", type: "Partial"})
    }
}
