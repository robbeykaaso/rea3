import QtQuick 2.12

Grid{
    property string name
    property Component com: Component{
            Rectangle{
                width: parent.width / parent.columns
                height: parent.height / parent.rows
                color: "transparent"
                border.color: "black"
            }
        }
    property var scale: {"sum": 1}
    property int viewcount: 0
    columns: 1
    rows: 1
    spacing: - 1
    id: root

    function autoRowsAndColumns(aSum){
        var cnt = Math.ceil((Math.sqrt(aSum)))
        columns = cnt
        rows = Math.ceil(aSum / cnt)
    }

    function addView(aIndex){
        com.createObject(root, {name: name + "_gridder" + aIndex, index: aIndex})
    }

    function deleteView(aIndex){
        children[aIndex].destroy()
    }

    function updateViewCount(aScale){
        var sum = 0
        if (aScale["layout"] || aScale["sum"]){
            if (aScale["layout"]){
                rows = aScale["layout"][0]
                columns = aScale["layout"][1]
                sum = rows * columns
            }
            if (aScale["sum"])
                sum = aScale["sum"]
            if (!aScale["layout"])
                autoRowsAndColumns(sum)
            if (sum >= 0){
                var del = sum - viewcount
                var i
                if (del > 0){
                    viewcount += del
                    for (i = 0; i < del; ++i){
                        addView(viewcount - del + i)
                    }
                }else if (del < 0){
                    del = - del
                    viewcount -= del
                    for (i = 0; i < del; ++i)
                        deleteView(viewcount + del - i - 1)
                }
            }
        }
        if (aScale["invisible"]){
            if (!sum)
                sum = children.length
            for (var j = 0; j < children.length; ++j){
                children[j].visible = !aScale["invisible"][j]
                if (aScale["invisible"][j])
                    sum--
            }
            rows = Math.ceil(sum / columns)
        }
    }

    Component.onCompleted: {
        updateViewCount(scale)
    }
}
