import QtQuick 2.12
import QtQuick.Controls 2.12
import QtCharts 2.3
import QtQuick.Window 2.12

ChartView {
    width: parent.width
    height: parent.height * 0.8
    antialiasing:true
    legend.alignment:Qt.AlignBottom
    legend.visible: false
    //titleColor: UIManager.fontColor
    //titleFont.pixelSize: UIManager.fontDespSize
    //titleFont.family: UIManager.fontFamily
    backgroundColor: "transparent"
    ValueAxis{
        id: axisY
        min: 0
        max: 15
        //labelsColor: UIManager.fontColor
        labelsFont.pixelSize: 10
        //labelsFont.family: UIManager.fontFamily
    }
    ValueAxis{
        id: axisX
        min: 0
        max: 3
        tickCount: 6
        //labelsColor: UIManager.fontColor
        labelsFont.pixelSize: 10
        //labelsFont.family: UIManager.fontFamily
    }
    AreaSeries {
        axisX: axisX
        axisY: axisY
        color: "#1f9b98"
        upperSeries: LineSeries{
            id: data
            /*XYPoint {x: 0; y: 5}
            XYPoint {x: 0.1; y: 5}
            XYPoint {x: 0.1; y: 8}
            XYPoint {x: 0.2; y: 8}
            XYPoint {x: 0.2; y: 2}
            XYPoint {x: 0.3; y: 2}*/
        }
        borderColor: "transparent"
    }

    function drawHistomgram(aHistogram){
        data.clear()
        axisX.max = aHistogram.length
        var mx = 0
        for (var j in aHistogram){
            if (aHistogram[j] > mx)
                mx = aHistogram[j]
            var idx = parseInt(j)
            data.append(idx, 0)
            data.append(idx, aHistogram[j])
            data.append(idx + 1, aHistogram[j])
            data.append(idx + 1, 0)
        }
        axisY.max = mx + 1
    }
}
