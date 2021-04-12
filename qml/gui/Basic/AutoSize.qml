import QtQuick 2.12

Item{
    TextMetrics{
        id: metc
    }
    Component.onCompleted: {
        var clds = []
        var mx = 0
        for (var i in parent.children){
            var cld = parent.children[i]
            if (cld.caption !== undefined && cld.ratio !== undefined){
                metc.text = cld.caption.text
                metc.font = cld.caption.font
                mx = Math.max(mx, metc.width)
                clds.push(cld)
            }
        }
        for (var j in clds)
            clds[j].caption.width = mx + 5
    }
}
