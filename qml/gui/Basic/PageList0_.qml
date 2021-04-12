import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Basic"

Column {
    id:root
    property var title: ["attr0", "attr1", "attr2"]
    property var selects: []
    property string selcolor: "lightskyblue"
    property string dfltcolor: "transparent"
    property string fontclr: "black"

    signal selected
    property var tr

    property var entries
    property int select: - 1
    property int entrycount: 0
    property int pageindex: 0
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
        height: parent.height - 60
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
            states: State {
                //name: "Current"
                when: itm.ListView.isCurrentItem
                PropertyChanges { target: itm; x: 0; scale: 1.1}
            }
            transitions: Transition {
                NumberAnimation { easing.type: Easing.Linear; properties: "scale"; duration: 200 }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: function(aEvent){
                    var idx
                    var range = [entrycount * pageindex, entrycount * (pageindex + 1)]
                    if (aEvent.modifiers === Qt.ShiftModifier){
                        var st = Math.min(select, index + range[0]), ed = Math.max(select, index + range[0])
                        for (var j = selects.length - 1; j >= 0; j--)
                            if (selects[j] < st || selects[j] > ed){
                                if (selects[j] >= range[0] && selects[j] < range[1])
                                    mdl.get(selects[j] % entrycount).clr = dfltcolor
                                selects.splice(j, 1)
                            }
                        for (var k = st; k <= ed; ++k)
                            if (selects.indexOf(k) < 0){
                                if (k >= range[0] && k < range[1])
                                    mdl.get(k % entrycount).clr = selcolor
                                selects.push(k)
                            }
                    }else if (aEvent.modifiers === Qt.ControlModifier){
                        if (index + range[0] != select){
                            idx = selects.indexOf(index + range[0])
                            if (idx < 0){
                                selects.push(index + range[0])
                                mdl.get(index).clr = selcolor
                            }else{
                                selects.splice(idx, 1)
                                mdl.get(index).clr = dfltcolor
                            }
                        }
                    }else{
                        for (var i in selects){
                            if (select !== selects[i] && selects[i] >= range[0] && selects[i] < range[1])
                                mdl.get(selects[i] % entrycount).clr = dfltcolor
                        }
                        select = index + range[0]
                        view.currentIndex = select >= entrycount * pageindex && select < entrycount * (pageindex + 1) ? select % entrycount : - 1
                        selects = [select]
                    }
                    root.selected()
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
            id: scr
            //policy: ScrollBar.AlwaysOn
        }
        function updateScrollPolicy(){
            if (height > 0){
                if (height < count * 30)
                    scr.policy = ScrollBar.AlwaysOn
                else
                    scr.policy = ScrollBar.AsNeeded
            }
        }
        onCountChanged: {
            updateScrollPolicy()
        }
        onHeightChanged: {
            updateScrollPolicy()
        }
    }

    PageSelect{
        page_text: (pageindex + 1).toString()
        page_suffix: entries ? "/" + Math.ceil(entries.length / entrycount) : ""
        page_title: tr("Page") + ": "
        onSelectSpecificPage: function(aIndex){
            if (entries){
                pageindex = Math.min(Math.max(1, aIndex), Math.ceil(entries.length / entrycount)) - 1
                updatePage()
            }
        }
        onSelectNeighbourPage: function(aNext){
            selectNextPage(aNext)
        }
    }

    function updatePage(){
        mdl.clear()
        var range = [entrycount * pageindex, entrycount * (pageindex + 1)]
        for (var i = range[0]; i < range[1]; ++i){
            var idx = i.toString()
            if (entries[idx]){
                var ent = entries[idx]["entry"]
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
        select = selects.length > 0 ? selects[0] : - 1
        view.currentIndex = select >= entrycount * pageindex && select < entrycount * (pageindex + 1) ? select % entrycount : - 1
    }

    function selectNextPage(aNext){
        pageindex = Math.min(Math.max(1, parseInt(aNext? pageindex + 1 + 1 : pageindex + 1 - 1)), Math.ceil(entries.length / entrycount)) - 1
        updatePage()
    }

    function selectNext(aNext){
        if (entries.length > 0){
            var range = [entrycount * pageindex, entrycount * (pageindex + 1)]
            for (var i in selects){
                if (select !== selects[i] && selects[i] >= range[0] && selects[i] < range[1])
                    mdl.get(selects[i] % entrycount).clr = dfltcolor
            }
            selects = [select]
            var refresh = false
            if (aNext){
                if (selects[0] % entrycount == entrycount - 1 && selects[0] < entries.length - 1){
                    pageindex++
                    refresh = true
                }
                selects[0] = Math.min(selects[0] + 1, entries.length - 1)
            }else{
                if (selects[0] % entrycount == 0 && selects[0] > 0){
                    pageindex--
                    refresh = true
                }
                selects[0] = Math.max(selects[0] - 1, 0)
            }
            if (refresh)
                updatePage()
            else{
                select = selects[0]
                view.currentIndex = select >= entrycount * pageindex && select < entrycount * (pageindex + 1) ? select % entrycount : - 1
            }
            root.selected()
        }
    }

    function selectAll(){
        var st = 0, ed = entries.length
        var range = [entrycount * pageindex, entrycount * (pageindex + 1)]
        for (var k = st; k < ed; ++k)
            if (selects.indexOf(k) < 0){
                if (k >= range[0] && k < range[1])
                    mdl.get(k % entrycount).clr = selcolor
                selects.push(k)
            }
    }   

    function updateModel(aInput){
        if (aInput["title"])
            title = aInput["title"]
        if (aInput["selects"])
            selects = aInput["selects"]
        entrycount = aInput["entrycount"] || 10
        if (aInput["pageindex"] !== undefined)
            pageindex =  aInput["pageindex"]

        fontclr = aInput["fontclr"] || "black"
        entries = aInput["data"]
        updatePage()
    }
}
