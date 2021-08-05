import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import "../../Basic"

Column{
    id: root
    property int treelayer
    property string key
    property string tag: "object"
    property string caption: " "
    property alias nodechild: cld
    property bool opt_add: false
    property var scr_root
    spacing: 5
    Row{
        //width: parent.width
        //height: 20
        spacing: 5
        Text{
            text: tag == "object" ? "{" + caption + "}" : "[" + caption + "]"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cld.visible = !cld.visible
                }
            }
            font.pixelSize: 12
        }
        Button{
            visible: opt_add
            text: "+"
            width: 12
            height: width
            contentItem: Text{
                text: parent.text
                color: "white"
                font.pixelSize: 14
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                typeselect.show()
            }
        }
        TWindow{
            id: typeselect
            width: Screen.desktopAvailableWidth * 0.22
            height: Screen.desktopAvailableHeight * 0.22
            caption: qsTr("Select Type")

            ButtonGroup{
                id: btngrp
            }

            content:
                Rectangle{
                    color: "gray"
                    anchors.fill: parent
                    ScrollView{
                        anchors.fill: parent
                        contentHeight: 200
                        contentWidth: parent.width
                        clip: true

                        Column{
                            height: 150
                            width: parent.width
                            spacing: height * 0.05
                            topPadding: spacing

                            Repeater{
                                id: sel
                                model: ListModel{
                                    ListElement {cap: "double/integer"; chk: false}
                                    ListElement {cap: "string"; chk: true}
                                    ListElement {cap: "boolean"; chk: false}
                                    ListElement {cap: "array"; chk: false}
                                    ListElement {cap: "object"; chk: false}
                                }
                                Row{
                                    height: 25
                                    width: parent.width
                                    spacing: 5
                                    leftPadding: spacing
                                    Radio{
                                        text: cap
                                        checked: chk
                                        height: parent.height
                                        ButtonGroup.group: btngrp
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Edit{
                                        visible: root.tag == "object"
                                        caption.text: qsTr("key")
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Edit{
                                        //visible: root.tag ==
                                        visible: cap == "double/integer" || cap == "string"
                                        caption.text: qsTr("value")
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Check{
                                        visible: cap == "boolean"
                                        caption.text: qsTr("value")
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }

                    }
                }
            footbuttons: [{cap: "Cancel", func: function(){close()}},
            {cap: "OK", func: function(){
                var tp, ky
                for (var i = 0; i < sel.count; ++i){
                    if (sel.itemAt(i).children[0].checked){
                        tp = sel.itemAt(i).children[0].text
                        ky = sel.itemAt(i).children[1].input.text
                        break
                    }
                }
                if (tag == "array"){
                    ky = cld.children.length - Object.keys(cld.deleted).length
                    if (sel.itemAt(i).children[3].visible)
                        addJsonChild(tp, ky, sel.itemAt(i).children[3].check.checked)
                    else if (sel.itemAt(i).children[2].visible){
                        if (tp !== "string")
                            tp = "double"
                        addJsonChild(tp, ky, sel.itemAt(i).children[2].input.text);
                    }
                    else
                        addJsonChild(tp, ky)
                }
                else{
                    for (var j = 0; j < cld.children.length; ++j)
                        if (ky === cld.getChildValue(j)){
                            Pipelines().run("popMessage", {title: "warning", text: "Duplicated key!"})
                            return
                        }

                    if (sel.itemAt(i).children[3].visible)
                        addJsonChild(tp, ky, sel.itemAt(i).children[3].check.checked)
                    else if (sel.itemAt(i).children[2].visible){
                        if (tp !== "string")
                            tp = "double"
                        addJsonChild(tp, ky, sel.itemAt(i).children[2].input.text);
                    }
                    else
                        addJsonChild(tp, ky)
                }
            }}]
        }
    }
    Column{
        id: cld
        property var deleted
        width: parent.width
        //height: 55
        spacing: 5
        leftPadding: 10
        visible: true
        /*CustomEdit1{
            caption.text: qsTr("hello2")
            editbackground.width: 100
            editbackground.height: 25
        }
        CustomEdit1{
            caption.text: qsTr("hello3")
            editbackground.width: 100
            editbackground.height: 25
        }*/

        function getChildType(){
            if (children.length > 0)
                if (children[0].children[1] instanceof Check)
                    return "boolean"
                else
                    return children[0].children[1].tag
            else
                return ""
        }

        function getChildValue(aIndex){
            var chd = children[aIndex].children[1]
            if (chd instanceof Edit || chd instanceof Check || chd instanceof Combo)
                return chd.caption.text
            else
                return chd.caption
        }

        Component.onCompleted: {
            deleted = {}
        }
    }

    /*ComboBox1{
        onValueChanged: {
            value
        }
    }*/

    function extractKeyChain(){
        var ret = key
        if (parent instanceof Row && parent.parent instanceof Column && parent.parent.parent instanceof TreeNode)
            ret = parent.parent.parent.extractKeyChain() + ";" + ret
        return ret
    }

    function getType(aValue){
        if (!(aValue instanceof Object)){
            var tp = typeof aValue
            if (tp == "boolean"){
                return "boolean"
            }else{
                if (tp === "string")
                    return "string"
                else
                    return "double"
            }
        }else{
            if (aValue instanceof Array)
                return "array"
            else
                return "object"
        }
    }

    function modifyGUI(aKeys, aOpt, aValue, aStyle){
        var i
        if (aKeys.length === 1){
            if (aOpt === "add")
                addJsonChild(getType(aValue), aKeys[0], aValue, aStyle)
            else{
                for (i = 0; i < cld.children.length; ++i)
                    if (!cld.deleted[cld.children[i]]){
                        var istar = false
                        if (cld.children[i].children[1] instanceof Check){
                            istar = cld.children[i].children[1].key === aKeys[0]
                            if (aOpt !== "del")
                                cld.children[i].children[1].check.checked = aValue
                        }else if (cld.children[i].children[1] instanceof Combo){
                            istar = cld.children[i].children[1].key === aKeys[0]
                            if (aOpt !== "del"){
                                var cmb = cld.children[i].children[1].combo
                                cmb.currentIndex = cmb.model.indexOf(aValue)
                            }
                        }else if (cld.children[i].children[1] instanceof Edit){
                            istar = cld.children[i].children[1].key === aKeys[0]
                            if (aOpt !== "del")
                                cld.children[i].children[1].input.text = aValue
                        }else if (cld.children[i].children[1] instanceof TreeNode)
                            istar = cld.children[i].children[1].key === aKeys[0]
                        if (istar){
                            if (aOpt === "del")
                                cld.children[i].children[0].deleteTreeNode()
                            break
                        }
                    }

            }
        }else if (aKeys.length > 1){
            for (i = 0; i < cld.children.length; ++i)
                if (!cld.deleted[cld.children[i]] && cld.children[i].children[1] instanceof TreeNode && cld.children[i].children[1].key === aKeys[0]){
                    var kys = aKeys
                    kys.splice(0, 1)
                    cld.children[i].children[1].modifyGUI(kys, aOpt, aValue, aStyle)
                    break
                }
        }
    }

    function addJsonChild(aType, aKey, aValue, aStyle, aInitialize){
        var ret
        var src = "import QtQuick 2.12; import '../../Basic'; import TextFieldDoubleValidator 1.0;"
        src += "Row{"
        if (aStyle && aStyle["jsst"] && aStyle["jsst"]["invisible"])
            src += "visible: false;"
        src += "spacing: 5;"
        src += "TreeNodeDelete{"
        if (aStyle && aStyle["jsst"] && aStyle["jsst"]["opt_del"]){
            //src += "enabled: false;"
            //src += "text: '';"
            //src += "color: 'transparent';"
            src += "visible: false;"
        }
        var cap = aKey
        if (aStyle && aStyle["jsst"] && aStyle["jsst"]["caption"] && aStyle["jsst"]["caption"] !== "")
            cap = aStyle["jsst"]["caption"]
        if (aType === "boolean"){
            src += "anchors.verticalCenter: parent ? parent.children[1].verticalCenter : undefined;}"
            src += "Check{"
            //if (tag !== "array")
            src += "property string key: '" + aKey + "';"
            src += "caption.text: '" + cap + "';"
            src += "height: 20;"
            src += "check.checked: " + aValue + ";"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["val_script"])
                src += aStyle["jsst"]["val_script"]
            else
                src += "check.onCheckedChanged: {Pipelines().run(scr_root.contentChildren[0].key + 'treeViewGUIModified', {key: parent.parent.parent.extractKeyChain() + ';' + key, val: check.checked});}"
            src += "}"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["comment"] && aStyle["jsst"]["comment"] !== ""){
                src += "TreeNodeTag{anchors.verticalCenter: parent ? parent.children[1].verticalCenter : undefined;"
                src += "comment: '" + aStyle["jsst"]["comment"] + "';"
                src += "}"
            }
            src += "AutoSize{}}"
            ret = Qt.createQmlObject(src, cld)
            scr_root.contentHeight += 40
        }else if (aType === "string" || aType === "double"){
            src += "anchors.verticalCenter: parent ? parent.children[1].verticalCenter : undefined;}"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["val_type"] === "combo"){
                src += "Combo{"
                //if (tag !== "array")
                src += "property int mdytick: 0;"
                src += "property string key: '" + aKey + "';"
                src += "caption.text: '" + cap + "';"
                src += "combo.width: 60;"
                //src += "combo.height: 35;"
                var val = ""
                var cur = 0;
                for (var i in aStyle["jsst"]["val_value"]){
                    if (val != "")
                        val += ","
                    else
                        if (typeof aStyle["jsst"]["val_value"][i] != "string")
                            src += "property string tag: 'double';"
                    if (aStyle["jsst"]["val_value"][i] === aValue)
                        src += "combo.currentIndex: " + cur + ";"
                    val += "'" + aStyle["jsst"]["val_value"][i] + "'"
                    cur++
                }
                src += "combo.model: [" + val + "];"
                if (aStyle["jsst"]["val_script"]){
                    src += aStyle["jsst"]["val_script"]
                }
                else{
                    src += "combo.onCurrentTextChanged: {"
                    src += "if (mdytick++){"
                    src += "Pipelines().run(scr_root.contentChildren[0].key + 'treeViewGUIModified', {key: parent.parent.parent.extractKeyChain() + ';' + key, val: combo.currentText});"
                    //src += "console.log('hi2');"
                    src += "}}"
                }
            }else{
                src += "Edit{"
                src += "property string tag: '" + aType + "';"
                //if (tag !== "array")
                src += "property string key: '" + aKey + "';"
                src += "caption.text: '" + cap + "';"
                src += "input.text: '" + aValue + "';"
                if (aStyle && aStyle["jsst"] && aStyle["jsst"]["val_type"] === "regExp"){
                    if (aStyle["jsst"]["val_value"] !== "")
                        src += "input.validator: RegExpValidator{regExp: " + aStyle["jsst"]["val_value"] + ";}"
                }else if (aType === "double"){
                    if (aStyle && aStyle["jsst"] && aStyle["jsst"]["val_type"] === "range"){
                        src += "input.validator: TextFieldDoubleValidator{bottom:" + aStyle["jsst"]["val_value"][0] + "; top:" + aStyle["jsst"]["val_value"][1] + ";}"
                    }else
                        src += "input.validator: DoubleValidator{}"
                }
                if (aStyle && aStyle["jsst"] && aStyle["jsst"]["val_script"])
                    src += aStyle["jsst"]["val_script"]
                else{
                    //src += "Keys.onPressed: {"
                    //src += "if (event.key === 16777220)"
                    //if (aType === "string")
                    //    src += "UIManager.setCommand({signal2: 'treeViewGUIModified', type: 'nullptr', param: {keys: parent.parent.parent.extractKeyChain() + ';' + key, value: input.text}}, null);"
                    //else
                    //    src += "UIManager.setCommand({signal2: 'treeViewGUIModified', type: 'nullptr', param: {keys: parent.parent.parent.extractKeyChain() + ';' + key, value: parseFloat(input.text)}}, null);"
                    //src += "}"

                    if (aType === "string")
                        src += "input.onTextEdited: {Pipelines().run(scr_root.contentChildren[0].key + 'treeViewGUIModified', {key: parent.parent.parent.extractKeyChain() + ';' + key, val: input.text});}"
                    else
                        src += "input.onTextEdited: {Pipelines().run(scr_root.contentChildren[0].key + 'treeViewGUIModified', {key: parent.parent.parent.extractKeyChain() + ';' + key, val: parseFloat(input.text)});}"
                }
            }
            src += "}"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["comment"] && aStyle["jsst"]["comment"] !== ""){
                src += "TreeNodeTag{anchors.verticalCenter: parent ? parent.children[1].verticalCenter : undefined;"
                src += "comment: '" + aStyle["jsst"]["comment"] + "';"
                src += "}"
            }
            src += "AutoSize{}}"
            ret = Qt.createQmlObject(src, cld)
            scr_root.contentHeight += 40
        }else{
            src += "}"
            src += "TreeNode{"
            src += "tag: '" + aType + "';"
            //if (tag !== "array")
            src += "key: '" + aKey + "';"
            src += "caption: '" + cap + "';"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["opt_add"])
                src += "opt_add: false;"
            src += "}"
            if (aStyle && aStyle["jsst"] && aStyle["jsst"]["comment"] && aStyle["jsst"]["comment"] !== ""){
                src += "TreeNodeTag{"
                src += "comment: '" + aStyle["jsst"]["comment"] + "';"
                src += "}"
            }
            src += "}"

            ret = Qt.createQmlObject(src, cld)
            ret.children[1].scr_root = scr_root
            ret.children[1].treelayer = treelayer + 1
            //if (!aInitialize)
            //    ret.children[1].nodechild.visible = true
            scr_root.contentHeight += 37
        }
        scr_root.contentWidth = Math.max(scr_root.contentWidth, (treelayer + 1) * 10 + treelayer * 17 + ret.children[1].x + ret.children[1].width + 30)
        if (!aInitialize)
            Pipelines().run(scr_root.contentChildren[0].key + "treeViewGUIModified", {key: extractKeyChain() + ";" + aKey, val: aValue, type: "add"})
//        console.log(ret.children[1].x + ";" + ret.children[1].width)
//        if (cld.children.length - Object.keys(cld.deleted).length > 1)
//            scr_root.contentHeight += 5
        return ret
    }

    function buildArray(){
        var ret = []
        for (var i = 0; i < cld.children.length; ++i)
            if (!cld.deleted[cld.children[i]]){
                var cld0 = cld.children[i].children[1]
                if (cld0 instanceof Edit){
                    if (cld0.tag === "double")
                        ret.push(parseFloat(cld0.input.text))
                    else
                        ret.push(cld0.input.text)
                }else if (cld0 instanceof Check){
                    ret.push(cld0.check.checked)
                }else if (cld0 instanceof Combo){
                    if (cld0.tag === "double")
                        ret.push(parseFloat(cld0.combo.currentText))
                    else
                        ret.push(cld0.combo.currentText)
                }else{
                    if (cld0.tag === "object")
                        ret.push(cld0.buildObject())
                    else
                        ret.push(cld0.buildArray())
                }
            }
        return ret
    }

    function buildObject(){
        var ret = {}
        for (var i = 0; i < cld.children.length; ++i)
            if (!cld.deleted[cld.children[i]]){
                var cld0 = cld.children[i].children[1]
                if (cld0 instanceof Edit){
                    if (cld0.tag === "double")
                        ret[cld0.key] = parseFloat(cld0.input.text)
                    else
                        ret[cld0.key] = cld0.input.text
                }else if (cld0 instanceof Check){
                    //console.log("xxxxx: " + cld0.key + ";" + cld0.tag)
                    ret[cld0.key] = cld0.check.checked
                }else if (cld0 instanceof Combo){
                    if (cld0.tag === "double")
                        ret[cld0.key] = parseFloat(cld0.combo.currentText)
                    else
                        ret[cld0.key] = cld0.combo.currentText
                }else{
                    if (cld0.tag === "object")
                        ret[cld0.key] = cld0.buildObject()
                    else
                        ret[cld0.key] = cld0.buildArray()
                }
            }
        return ret
    }

    function doBuildGUI(aKey, aValue, aStyle){
        //console.log("hi")
        var tp = getType(aValue)
        var nd = addJsonChild(tp, aKey, aValue, aStyle ? aStyle[aKey] : undefined, true)
        if ((aValue instanceof Object))
            nd.children[1].buildGUI(aValue, aStyle ? aStyle[aKey] : undefined)
    }

    function buildGUI(aJson, aStyle){
        if (aJson["jsst"]){
            doBuildGUI("jsst", aJson["jsst"], aStyle)
        }
        for (var i in aJson)
            if (i !== "jsst")
                doBuildGUI(i, aJson[i], aStyle)
    }

    function clearChildren(){
        cld.deleted = {}
        for (var i = 0; i < cld.children.length; ++i)
            if (!cld.deleted[cld.children[i]])
                cld.children[i].children[0].deleteTreeNode(true)
    }
}
