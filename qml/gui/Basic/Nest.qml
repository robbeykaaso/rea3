import QtQuick 2.12
import QtQuick.Layouts 1.12
//https://stackoverflow.com/questions/34027727/how-can-i-create-a-qml-gridlayout-with-items-of-proportionate-sizes
GridLayout{
    property var size: []
    rowSpacing: 0
    columnSpacing: 0

    function fitLayout(){
        if (width > 0 && height > 0){
            for (var i = 0; i < children.length; ++i)
                if (2 * i + 1 < size.length){
                    children[i].Layout.rowSpan = size[2 * i]
                    children[i].Layout.columnSpan = size[2 * i + 1]
                    children[i].Layout.preferredWidth = children[i].Layout.columnSpan * width / columns
                    children[i].Layout.preferredHeight = children[i].Layout.rowSpan * height / rows
                }
        }
    }

    onWidthChanged: {
        fitLayout()
    }
    onHeightChanged: {
        fitLayout()
    }
}
