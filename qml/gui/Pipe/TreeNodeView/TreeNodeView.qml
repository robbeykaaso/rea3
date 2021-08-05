import QtQuick 2.12
import QtQuick.Controls 2.5

ScrollView{
    id: scrrange
    property string root: ""
    width: parent.width
    height: parent.height - 35
    contentHeight: 0
    contentWidth: parent.width
    clip: true

    function buildGUI(aData){
        rt.clearChildren()
        contentHeight = 0
        rt.buildGUI(aData)
    }

    function generateData(){
        return rt.buildObject()
    }

    TreeNode{
        id: rt
        caption: qsTr("root")
        key: root
        width: parent.width
        height: 500
        spacing: 5
        topPadding: spacing
        leftPadding: 10
        scr_root: scrrange
        treelayer: 0

        function generateJSST(aContent){
            var stl = {}
            var idx = 0
            for (var i in aContent){
                var tp = typeof aContent[i]
                if (aContent[i] instanceof Object){
                    var isarr = false
                    if (aContent[i] instanceof Array){
                        var arr = {}
                        for (var j in aContent[i]){
                            arr[j] = aContent[i][j]
                        }
                        //console.log(aContent[i])
                        aContent[i] = arr
                        isarr = true
                    }
                    stl[i] = generateJSST(aContent[i])
                    aContent[i]["jsst"] = {
                        invisible: false,
                        comment: "",
                        caption: "",
                        opt_del: true,
                        opt_add: !isarr
                    }
                }else if (tp == "boolean"){
                    aContent[i] = {jsst: {
                            invisible: false,
                            comment: "",
                            caption: "",
                            opt_del: true
                        }}
                }else{
                    aContent[i] = {jsst: {
                            invisible: false,
                            comment: "",
                            caption: "",
                            opt_del: true,
                            val_type: "regExp",
                            val_value: ""
                        }}
                    var mdl = ["regExp", "combo"]
                    if (tp !== "string")
                        mdl = ["regExp", "range"]

                    var tmp = ""
                    stl[i] = {jsst: {
                                val_type: {
                                    jsst: {
                                        val_type: "combo",
                                        val_value: mdl,
                                        val_script: "combo.onCurrentTextChanged: {" +
                                            "if (parent.parent.children.length > 5){" +
                                                "if (combo.currentText === 'combo'){" +
                                                    "parent.parent.children[5].children[0].deleteTreeNode();" +
                                                    "parent.parent.parent.addJsonChild('array', 'val_value');" +
                                                "}else if (combo.currentText === 'range'){" +
                                                    "parent.parent.children[5].children[0].deleteTreeNode();" +
                                                    "var reg = parent.parent.parent.addJsonChild('array', 'val_value');" +
                                                    "reg.children[1].addJsonChild('double', 'min', 0);" +
                                                    "reg.children[1].addJsonChild('double', 'max', 1);" +
                                                "}else{" +
                                                    "parent.parent.children[5].children[0].deleteTreeNode();" +
                                                    "parent.parent.parent.addJsonChild('string', 'val_value', '');" +
                                                "}" +
                                            "}" +
                                        "}"
                                    }
                                }
                             }}
                }
                ++idx
            }
            return stl
        }

        Component.onCompleted: {
            Pipelines().add(function(aInput){
                var dt = aInput.data()
                clearChildren()
                contentHeight = 0
                buildGUI(dt["data"], dt["style"])
                aInput.out()
            }, {name: key + "loadTreeView", type: "Partial"})

            Pipelines().add(function(aInput){
                var dt = aInput.data()
                modifyGUI(dt["key"], dt["type"], dt["val"], dt["style"])
                aInput.out()
            }, {name: key + "modifyTreeViewGUI", type: "Partial"})

            Pipelines().add(function(aInput){
                //console.log(aInput["key"] + ";" + aInput["val"] + ";" + aInput["type"])
                aInput.out()
            }, {name: key + "treeViewGUIModified"})

            Pipelines().add(function(aInput){
                aInput.setData(buildObject()).out()
            }, {name: key + "saveTreeView", type: "Partial"})

            Pipelines().add(function(aInput){
                var dt = aInput.data()
                var stl = generateJSST(dt)
                aInput.setData({data: dt, style: stl}).out()
            }, {name: key + "styleTreeView", type: "Partial"})

          /*  var style = "styleTreeView"
            Pipeline2.add(function(aInput){
                var content = buildObject()
                var stl = generateJSST(content)
                return {data: {data: content, style: stl}, out: {}}
            }, {name: key + style})
            .nextL("loadTreeView")
            //.nextL("saveTreeView")
            .next(function(aInput){
                return {out: [{
                            out: {data: aInput["style"], path: "style1.json"},
                        }]}
            })
            .nextL("json2stg")
            .nextL("writeJson")*/
        }
    }
}
