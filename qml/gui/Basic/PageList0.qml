import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Basic"

Column {
    id:root
    property var title: ["attr0", "attr1", "attr2"]
    property var selects: []
    property string selcolor: "lightskyblue"
    property string dfltcolor: "transparent"
    property string titcolor: "transparent"
    property int titfontsize: 14
    property int itmfontsize: 12
    property string fontclr: "black"

    signal selected
    property var tr

    property var entries
    property int select: - 1
    property int entrycount: 0
    property int pageindex: 0

    property var tableLists: []

    onWidthChanged: updateChildrenWidth()

    function updateChildrenWidth() {
        for (var i = 0; i < rep.children.length; ++i) {
            rep.children[i].width = parent.width / title.length + (i == title.length - 1 ? 0 : 1)

            var lists = tableLists.filter(v => v.index === i)

            for (var j = 0; j < lists.length; ++j) {
                lists[j].component.width = rep.children[i].width
            }
        }
    }

    width: 100
    height: 200
    Row {
        width: parent.width
        height: 30
        spacing: -1
        id: rep

        Repeater {
            model: title.length
            delegate: Rectangle {
                border.color: "black"
                width: parent.width / title.length + (index == title.length - 1 ? 0 : 1)
                height: parent.height
                color: titcolor
                Text {
                    id: textV
                    text: tr(title[index])
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: titfontsize
                    elide: Text.ElideRight
                    clip: true
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    anchors.left: textV.right
                    visible: index !== title.length - 1

                    Rectangle {
                        width: 1
                        height: parent.height
                        color: 'black'
                    }

                    MouseArea {
                        hoverEnabled: true
                        width: 1
                        height: parent.height

                        property var origin_shape
                        property int coor_x
                        property int coor_y

                        onEntered: {
                            if (index !== title.length - 1) {
                                origin_shape = cursorShape
                                cursorShape = Qt.SplitHCursor
                            }
                        }
                        onExited: {
                            if (index !== title.length - 1) {
                                cursorShape = origin_shape
                            }
                        }
                        onPressed: function (aInput) {
                            coor_x = aInput["x"]
                            coor_y = aInput["y"]
                        }
                        onPositionChanged: function (aInput) {
                            if (aInput.buttons === Qt.LeftButton
                                    && index !== title.length - 1) {
                                var allWidth = rep.children[index].width
                                        + rep.children[index + 1].width

                                var x = aInput.x
                                if (x < 0 && Math.abs(
                                            x) < rep.children[index].width) {

                                    rep.children[index + 1].width
                                            = rep.children[index + 1].width + Math.abs(
                                                x)
                                    rep.children[index].width = allWidth
                                            - rep.children[index + 1].width
                                } else if (x > 0
                                           && x < rep.children[index + 1].width) {
                                    rep.children[index].width = rep.children[index].width + x
                                    rep.children[index + 1].width = allWidth
                                            - rep.children[index].width
                                }

                                for (var i = 0; i < rep.children.length; ++i) {
                                    var lists = tableLists.filter(v => v.index === i)

                                    for (var j = 0; j < lists.length; ++j) {
                                        lists[j].component.width = rep.children[i].width
                                    }
                                }
                            }
                        }
                    }
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
                        id: textR
                        width: parent.width / title.length + (index == title.length - 1 ? 0 : 1)
                        height: parent.height
                        text: entry[index]
                        color: fontclr
                        font.pixelSize: itmfontsize
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        clip: true

                        Component.onCompleted: {
                            var item = {
                                "index": index,
                                "component": textR
                            }
                            tableLists.push(item)
                        }
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
            border.width: 0
            //border.color: "#60f50a"
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
        background_color: selcolor
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
