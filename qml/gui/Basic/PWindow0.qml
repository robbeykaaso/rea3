import QtQuick 2.12
import "../Basic"

TWindow{
    property var tr
    property var customGetParam
    signal accept()
    signal reject()
    width: 320
    height: 100
    caption: tr("set parameter")

    Component{
        id: edit
        Item{
            property string cap
            property string key
            property string val
            property string trig
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 30
            Edit{
                anchors.fill: parent
                caption.text: tr(cap) + ":"
                input.text: val
                ratio: 0.4
            }
            /**/
            Component{
                id: trg
                MouseArea{
                    property string trig
                    anchors.fill: parent
                    enabled: trig != undefined
                    onClicked: {
                        if (customGetParam)
                        parent.children[0].input.text = customGetParam(trig)
                    }
                }
            }

            Component.onCompleted: {
                if (trig)
                    trg.createObject(this, {trig: trig})
            }
        }
    }

    Component{
        id: spn
        Spin{
            property string cap
            property string key
            property int val
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(cap) + ":"
            ratio: 0.4
            spin.value: val
        }
    }

    Component{
        id: chk
        Check{
            property string cap
            property string key
            property bool val: false
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(cap) + ":"
            ratio: 0.4
            check.checked: val
        }
    }

    Component{
        id: cmb
        Combo{
            property string cap
            property string key
            property var mdl
            property var proto
            property int val
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(cap) + ":"
            ratio: 0.4
            combo.model: mdl
            combo.currentIndex: val
        }
    }

    Component{
        id: cmbe
        ComboE{
            property string cap
            property string key
            property var mdl
            property string val
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            caption.text: tr(cap) + ":"
            ratio: 0.4
            combo.modellist: mdl
            combo.currentSelect: val
        }
    }

    content:
        Rectangle{
            anchors.fill: parent
            color: buttoncolor
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
            if (itms[i] instanceof Check)
                dt[itms[i].key] = itms[i].check.checked
            else if (itms[i] instanceof Spin)
                dt[itms[i].key] = itms[i].spin.value
            else if (itms[i] instanceof Combo)
                dt[itms[i].key] = itms[i].proto[itms[i].combo.currentIndex]
            else if (itms[i] instanceof ComboE)
                dt[itms[i].key] = itms[i].combo.currentSelect
            else if (itms[i] instanceof Item)
                dt[itms[i].key] = itms[i].children[0].input.text
        return dt
    }

    function showModel(aInput){
        caption = tr(aInput["title"]) || tr("set parameter")
        var cnt = aInput["content"]
        for (var i in cnt)
            if (cnt[i]["type"]){
                if (cnt[i]["type"] === "spin")
                    spn.createObject(sets, {key: i, val: cnt[i]["value"] || 0, cap: cnt[i]["caption"] || i})
                else if (cnt[i]["type"] === "check")
                    chk.createObject(sets, {key: i, val: cnt[i]["value"] || false, cap: cnt[i]["caption"] || i})
                else if (cnt[i]["type"] === "edit")
                    edit.createObject(sets, {key: i, val: cnt[i]["value"] || "", cap: cnt[i]["caption"] || i, trig: cnt[i]["trig"]})
                else if (cnt[i]["type"] === "combo")
                    cmb.createObject(sets, {key: i, mdl: cnt[i]["model"], proto: cnt[i]["proto"] || cnt[i]["model"], val: cnt[i]["value"] || 0, cap: cnt[i]["caption"] || i})
                else if (cnt[i]["type"] === "comboe")
                    cmbe.createObject(sets, {key: i, mdl: cnt[i]["model"], val: cnt[i]["value"] || (cnt[i]["model"].length > 0 ? cnt[i]["model"][0] : ""), cap: cnt[i]["caption"] || i})
            }else if (typeof cnt[i]["value"] == "number")
                spn.createObject(sets, {key: i, val: cnt[i]["value"] || 0, cap: cnt[i]["caption"] || i})
            else if (typeof cnt[i]["value"] == "boolean")
                chk.createObject(sets, {key: i, val: cnt[i]["value"] || false, cap: cnt[i]["caption"] || i})
            else
                edit.createObject(sets, {key: i, val: cnt[i]["value"] || "", cap: cnt[i]["caption"] || i, trig: cnt[i]["trig"]})
        setHeight(Object.keys(cnt).length * 30 + 100)
        show()
    }

    //{title: Pipeline2.tr("new task"), content: {name: "", type: {model: ["A", "B", "C"], index: 0}, checked: true}, tag: {tag: ""}}
}
