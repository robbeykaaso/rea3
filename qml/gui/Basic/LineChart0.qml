import QtQuick 2.12
import QtCharts 2.3

ChartView {
    property var content: [10, 20, 15, 30, 10, 60]

    antialiasing:true
    legend.alignment:Qt.AlignBottom
    legend.visible: false
    //titleColor: UIManager.fontColor
    //titleFont.pixelSize: UIManager.fontDespSize
    //titleFont.family: UIManager.fontFamily
    backgroundColor: "transparent"
    ValueAxis{
        id: valY
        //labelsColor: UIManager.fontColor
        labelsFont.pixelSize: 10
        //labelsFont.family: UIManager.fontFamily
        min: 0
    }
    ValueAxis{
        id: valX
        //labelsColor: UIManager.fontColor
        labelsFont.pixelSize: 10
        //labelsFont.family: UIManager.fontFamily
        min: 0
    }
    LineSeries {
        id: data
        axisX: valX
        axisY: valY
        XYPoint {x:0;y:2}
        XYPoint {x:1;y:2.3}
        XYPoint {x:2;y:1.5}
        XYPoint {x:3;y:1.7}
        XYPoint {x:4;y:1.3}
        XYPoint {x:5;y:1.1}
        XYPoint {x:6;y:0}
    }

    function updateGUI(){
        if (content === undefined)
            return;
        data.clear()
        var mx = 0;
        for (var i in content){
            if (content[i] > mx)
                mx = content[i]
            data.append(i, content[i]);
        }
        valX.max = content.length - 1
        valY.max = mx
    }

    function updateModel(aInput){
        content = aInput
        updateGUI()
    }

    Component.onCompleted: {
        updateGUI()
    }
}
