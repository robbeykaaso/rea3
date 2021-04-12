import QtQuick 2.12
import "../Basic"

TWindow{
    property var tr
    signal accept()
    signal reject()
    width: 320
    height: 100
    caption: tr("new object")

    Component{
        id: edit
        Edit{
            property string key
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(key) + ":"
            ratio: 0.4
        }
    }

    Component{
        id: spn
        Spin{
            property string key
            property int val: 1
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(key) + ":"
            ratio: 0.4
            spin.value: val
        }
    }

    Component{
        id: chk
        Check{
            property string key
            property bool val: false
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(key) + ":"
            ratio: 0.4
            check.checked: val
        }
    }

    Component{
        id: cmb
        Combo{
            property string key
            property var mdl
            property var val
            property int idx
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(key) + ":"
            ratio: 0.4
            combo.model: mdl
            combo.currentIndex: idx
        }
    }

    Component{
        id: cmbe
        ComboE{
            property string key
            property var mdl
            property string val
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(key) + ":"
            ratio: 0.4
            combo.modellist: mdl
            combo.currentSelect: val
        }
    }

    content:
        Rectangle{
            anchors.fill: parent
            color: bodycolor
            Column{
                id: sets
                anchors.fill: parent
                topPadding: 10
            }
        }
    footbuttons: [
        {cap: tr("OK"), func: function(){
            for (var j = 0; j < sets.children.length; ++j)
                sets.children[j].destroy()
            accept()
        }},
        {cap: tr("Cancel"), func: function(){
            for (var j = 0; j < sets.children.length; ++j)
                sets.children[j].destroy()
            reject()
        }}
    ]

    onClosed: {
        for (var j = 0; j < sets.children.length; ++j)
            sets.children[j].destroy()
        reject()
    }

    function outputModel(){
        var dt = {}
        var itms = sets.children
        for (var i = 0; i < itms.length; ++i)
            if (itms[i] instanceof Edit)
                dt[itms[i].key] = itms[i].input.text
            else if (itms[i] instanceof Check)
                dt[itms[i].key] = itms[i].check.checked
            else if (itms[i] instanceof Spin)
                dt[itms[i].key] = itms[i].spin.value
            else if (itms[i] instanceof Combo)
                dt[itms[i].key] = itms[i].val[itms[i].combo.currentIndex]
            else if (itms[i] instanceof ComboE)
                dt[itms[i].key] = itms[i].combo.currentSelect
        return dt
    }

    function showModel(aInput){
        caption = tr(aInput["title"]) || tr("new object")
        var cnt = aInput["content"]
        for (var i in cnt)
            if (typeof cnt[i] == "object"){
                if (cnt[i]["model"])
                    cmb.createObject(sets, {key: i, mdl: cnt[i]["model"], val: cnt[i]["value"] || cnt[i]["model"], idx: cnt[i]["index"]})
                else if (cnt[i]["modele"])
                    cmbe.createObject(sets, {key: i, mdl: cnt[i]["modele"], val: cnt[i]["value"] || (cnt[i]["modele"].length > 0 ? cnt[i]["modele"][0] : "" )})
            }
            else if (typeof cnt[i] == "number")
                spn.createObject(sets, {key: i, val: cnt[i]})
            else if (typeof cnt[i] == "boolean")
                chk.createObject(sets, {key: i, val: cnt[i]})
            else
                edit.createObject(sets, {key: i})
        setHeight(Object.keys(cnt).length * 30 + 100)
        show()
    }

    //{title: Pipeline2.tr("new task"), content: {name: "", type: {model: ["A", "B", "C"], index: 0}, checked: true}, tag: {tag: ""}}
}
