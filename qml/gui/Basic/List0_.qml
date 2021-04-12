import QtQuick 2.12
import QtQuick.Controls 2.0

Column {
    property string selectSuffix: ""
    property var title: ["attr0", "attr1", "attr2"]
    property var selects: []
    property string selcolor: "lightskyblue"
    property string dfltcolor: "transparent"
    property var tr
    signal selected

    width: 100
    height: 200
    Row {
        width: parent.width
        height: 30
        spacing: -1
        Repeater {
            model: title.length
            delegate: Rectangle {
                border.color: "black"
                width: parent.width / title.length + (index == title.length - 1 ? 0 : 1)
                height: parent.height
                Text {
                    text: tr(title[index])
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    ListView {
        id: view
        width: parent.width
        height: parent.height - 30
        clip: true
        delegate: Rectangle{
            id: itm
            width: parent.width
            height: 30
            color: clr
            Row {
                anchors.fill: parent
                spacing: -1
                Repeater {
                    model: title.length
                    delegate: Text {
                        width: parent.width / title.length + (index == title.length - 1 ? 0 : 1)
                        height: parent.height
                        text: entry[index]
                        color: fontclr
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        clip: true
                    }
                }
            }
            /*states: State {
                //name: "Current"
                when: itm.ListView.isCurrentItem
                PropertyChanges { target: itm; x: 0; scale: 1.1}
            }
            transitions: Transition {
                NumberAnimation { easing.type: Easing.Linear; properties: "scale"; duration: 200 }
            }*/
            MouseArea {
                anchors.fill: parent
                onClicked: function(aEvent){
                    if (aEvent.modifiers === Qt.ShiftModifier){
                        var st = Math.min(view.currentIndex, index), ed = Math.max(view.currentIndex, index)
                        for (var j = selects.length - 1; j >= 0; j--)
                            if (selects[j] < st || selects[j] > ed){
                                mdl.get(selects[j]).clr = dfltcolor
                                selects.splice(j, 1)
                            }
                        for (var k = st; k <= ed; ++k)
                            if (selects.indexOf(k) < 0){
                                mdl.get(k).clr = selcolor
                                selects.push(k)
                            }
                    }else if (aEvent.modifiers === Qt.ControlModifier){
                        if (index != view.currentIndex){
                            var idx = selects.indexOf(index)
                            if (idx < 0){
                                selects.push(index)
                                mdl.get(index).clr = selcolor
                            }else{
                                selects.splice(idx, 1)
                                mdl.get(index).clr = dfltcolor
                            }
                        }
                    }else{
                        for (var i in selects)
                            if (selects[i] !== view.currentIndex)
                                mdl.get(selects[i]).clr = dfltcolor
                        view.currentIndex = index
                        selects = [index]
                    }
                    selected()
                }
            }
        }
        highlight: Rectangle {
            width: parent ? parent.width : 0
            height: 30
            color: selcolor
            radius: 5
            border.width: 1
            border.color: "#60f50a"
        }
        highlightFollowsCurrentItem: true
        focus: true
        model: ListModel {
            id: mdl
        }
        ScrollBar.vertical: ScrollBar {
        }
    }

    function updateList(aInput){
        if (aInput["title"])
            title = aInput["title"]
        var entries = aInput["data"]
        if (aInput["selects"])
            selects = aInput["selects"]

        var cur = view.currentIndex
        var fontclr = aInput["fontclr"] || "black"
        if (aInput["index"]){
            var idxes = aInput["index"]
            for (var j in entries){
                var mdy = {entry: {},
                           clr: (selects.indexOf(idxes[j]) > 0) ? selcolor : dfltcolor,
                           fontclr: fontclr}
                var nw_ent = entries[j]["entry"]
                if (nw_ent){
                    for (var l in nw_ent)
                        mdy["entry"][l] = nw_ent[l]
                }else
                    mdy["entry"] = mdl.get(idxes[j]).entry
                mdl.set(idxes[j], mdy)
            }

        }else{
            mdl.clear()
            for (var i in entries){
                var ent = entries[i]["entry"]
                if (ent){
                    var itm = {entry: {},
                               clr: (selects.indexOf(parseInt(i)) > 0) ? selcolor : dfltcolor,
                               fontclr: fontclr}
                    for (var k in ent)
                        itm["entry"][k] = ent[k]
                    mdl.append(itm)
                }
            }
        }
        if (aInput["selects"])
            view.currentIndex = selects.length > 0 ? selects[0] : - 1
        else
            view.currentIndex = cur
    }
}
