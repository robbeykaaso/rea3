import QtQuick 2.12
import QtQuick.Controls 2.12
import Pipeline 1.0

Menu{
    id: boardmenu
    property var cmenu
    visible: false
    width: 120
    z: 9

    function executeCommand(aCaption){
        Pipeline.run("qml_" + cmenu[aCaption]["cmd"], cmenu[aCaption]["param"])
    }

    Component.onCompleted: {
        cmenu = {}
        Pipeline.find("qml_updateQSGMenu_" + parent.name).nextF(function(aInput){
            var dt = aInput.data()
            visible = dt["x"] !== undefined && dt["y"] !== undefined
            for (var i = count - 1; i >= 0; --i)
                removeItem(itemAt(i))
            if (dt["menu"]){
                var mn = dt["menu"]
                for (var j in mn){
                    cmenu[mn[j]["cap"]] = mn[j]
                    var src = "import QtQuick 2.12; import QtQuick.Controls 2.5;"
                    src += "MenuItem{"
                    src += "text: '" + Pipeline.tr(mn[j]["cap"]) + "';"
                    //src += "font.pixelSize: " + fontsize + ";"
                    src += "onClicked: {"
                    src += "boardmenu.executeCommand('" + mn[j]["cap"] + "');"
                    src += "}}"
                    insertItem(0, Qt.createQmlObject(src, boardmenu))
                }
            }
            if (visible){
                x = dt["x"]
                y = dt["y"]
            }
            aInput.out()
        })
    }
}
