import QtQuick 2.12
import "../Basic"
import Pipeline 1.0

Log0{
    property var type: ["system", "train"]
    property var level: ["info", "warning", "error"]

    property var content
    property string cur_type: type[0]
    property string cur_level: level[0]

    function updateLogList(){
        loglist.clear()
        var cur = content[cur_type][cur_level]
        for (var i in cur)
            loglist.append({msg: cur[i]})
    }

    loglevel.onCurrentIndexChanged: {
        var idx = loglevel.currentIndex
        if (level[idx] !== cur_level){
            cur_level = level[idx]
            updateLogList()
        }
    }

    closepanel.onClicked: {
        visible = false
    }

    Component.onCompleted: {
        Pipeline.add(function(aInput){
            var mdl = aInput.data()
            var tp = mdl["type"], lev = mdl["level"]
            if (type.indexOf(tp) >= 0 && level.indexOf(lev) >= 0){
                content[tp][lev].push(mdl["msg"])
                if (tp === cur_type && lev === cur_level)
                    loglist.append({msg: mdl["msg"]})
            }
            aInput.out()
        }, {name: "addLogRecord"})

        Pipeline.add(function(aInput){
            visible = true
            aInput.out()
        }, {name: "showLogPanel"})

        content = {}
        for (var i in type){
            content[type[i]] = {}
            for (var j in level)
                content[type[i]][level[j]] = []
        }

        typelist.model.clear()
        for (var k in type){
            typelist.model.append({cap: type[k], clr: typelist.model.count === 0 ? "red" : "transparent"})
            var func = function(aIndex){
                return function(){
                    for (var i = 0; i < typelist.model.count; ++i){
                        typelist.model.get(i).clr = i === aIndex ? "red" : "transparent"
                        if (i === aIndex)
                            cur_type = typelist.model.get(i).cap
                        updateLogList()
                    }
                }
            }
            var func2 = func(typelist.model.count - 1)
            typelist.itemAt(typelist.model.count - 1).clicked.connect(func2)
        }
        loglevel.model = level

        logmenu.parent.clicked.connect(function(aInput){
            logmenu.x = aInput.x
            logmenu.y = aInput.y
            logmenu.open()
        })
        clear.clicked.connect(function(aInput){
            content[cur_type][cur_level] = []
            loglist.clear()
        })
    }
}
