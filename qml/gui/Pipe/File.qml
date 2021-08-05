import QtQuick 2.12
import QtQuick.Dialogs 1.2

FileDialog {
    property string name
    property string service_tag
    property var service_scope

    title: Pipelines().tr("Please choose files")
    selectMultiple: true
    selectFolder: false
   // nameFilters: ["Image files (*.jpg *.png *.jpeg *.bmp)"] //"All files(*)"
    onAccepted: {
        var pths = []
        for (var i in fileUrls){
            //https://stackoverflow.com/questions/24927850/get-the-path-from-a-qml-url
            pths.push(decodeURIComponent(fileUrls[i].replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")))
            // unescape html codes like '%23' for '#'
            //pths += fileUrls[i].substring(8, fileUrls[i].length) + ";"
        }
        Pipelines().run(name + "_fileSelected", pths, service_tag, service_scope)
    }
    onRejected: {
        Pipelines().run(name + "_fileSelected", [], service_tag, service_scope)
    }

    Component.onCompleted: {
        Pipelines().add(function(aInput){
            aInput.out()
        }, {name: name + "_fileSelected", type: "Partial"})

        Pipelines().add(function(aInput){
            var mdl = aInput.data()
            selectFolder = mdl["folder"]
            if (selectFolder){
                title = mdl["title"] || Pipelines().tr("Please choose folder")
                nameFilters = ""
            }else{
                title = mdl["title"] || Pipelines().tr("Please choose files")
                nameFilters = mdl["filter"]
                selectMultiple = true
            }
            selectExisting = mdl["save"] === undefined
            service_tag = aInput.tag()
            service_scope = aInput.scope()
            open()
        }, {name: name + "_selectFile", type: "Delegate", delegate: name + "_fileSelected"})
    }
}
