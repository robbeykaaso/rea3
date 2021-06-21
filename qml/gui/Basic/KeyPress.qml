import QtQuick 2.12

Item {
    focus: parent.focus
    property var configs: []
    property var configLists: []

    onConfigsChanged: {
        configLists = []
        for (var i = 0; i < configs.length; ++i) {
            if (configs[i].getConfig)
                configLists = configLists.concat(configs[i].getConfig())
            else if (configs[i] instanceof Object)
                configLists.push(configs[i])
        }
    }

   // Keys.onPressed: function(e) {
   //     keyPressFunc(e)
   // }

    function keyPressFunc(e) {
        var index = -1

        // aItem.modifiers=>> 传入的是组合键
        // 举例： ctrl键为 Qt.ControlModifier, ctrl健同时shift键为 Qt.ControlModifier + Qt.ShiftModifier
        if (e.modifiers === Qt.NoModifier) {
            index = configLists.findIndex(function(aItem){ return aItem.key === e.key && !aItem.modifiers})
        } else {
            index = configLists.findIndex(function(aItem){ return aItem.key === e.key && aItem.modifiers === e.modifiers})
        }
        if (index !== -1) configLists[index].func()
    }

    function changeParentFocus(prt) {
        prt.focus = true
        if (prt.parent) {
            changeParentFocus(prt.parent)
        }
    }

    Component.onCompleted: changeParentFocus(parent)
}
