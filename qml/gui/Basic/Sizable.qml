import QtQuick 2.12

MouseArea{
    id: self
    property var origin_shape
    property int coor_x
    property int coor_y
    hoverEnabled: true
    width: parent instanceof Row ? 1 : parent.width
    height: parent instanceof Row ? parent.height : 1

    function getSibling(aPrev){
        for (var i = 0; i < parent.children.length; ++i){
            if (parent.children[i] === self)
                if (aPrev)
                    return parent.children[i - 1]
                else
                    return i < parent.children.length - 1 ? parent.children[i + 1] : null
        }
        return null
    }

    onEntered: {
        origin_shape = cursorShape
        if (parent instanceof Row)
            cursorShape = Qt.SplitHCursor
        else if (parent instanceof Column)
            cursorShape = Qt.SplitVCursor
    }
    onExited: {
        cursorShape = origin_shape
    }
    onPressed: function(aInput){
        coor_x = aInput["x"]
        coor_y = aInput["y"]
    }
    onPositionChanged: function(aInput){
        if (aInput.buttons === Qt.LeftButton){
            var sib = getSibling(true)
            if (parent instanceof Row){
                var delx = aInput["x"] - coor_x
                sib.del_size += delx
                getSibling(false).del_size -= delx
            }else if (parent instanceof Column){
                var dely = aInput["y"] - coor_y
                sib.del_size += dely
                getSibling(false).del_size -= dely
            }
        }
    }
}
