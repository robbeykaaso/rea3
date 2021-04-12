import QtQuick 2.12
import QtCharts 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import "../Basic"

ChartView{
    property var histogram: []
    property bool oneThreshold: false
    property alias thresholdmax: maxthreshold
    property alias thresholdmin: minthreshold

    signal thresholdChanged(double aMaxThreshold, double aMinThreshold)

    title: "Score"
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
        max: 1
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
            id: goodarea
            XYPoint {x: 0; y: 5}
            XYPoint {x: 0.1; y: 5}
            XYPoint {x: 0.1; y: 8}
            XYPoint {x: 0.2; y: 8}
            XYPoint {x: 0.2; y: 2}
            XYPoint {x: 0.3; y: 2}
        }
        borderColor: "transparent"
    }
    AreaSeries {
        axisX: axisX
        axisY: axisY
        color: "yellow"
        upperSeries: LineSeries{
            id: interarea
            XYPoint {x: 0.4; y: 8}
            XYPoint {x: 0.5; y: 8}
        }
        borderColor: "transparent"
    }
    AreaSeries {
        axisX: axisX
        axisY: axisY
        color: "#9e0a0a"
        upperSeries: LineSeries{
            id: badarea
            XYPoint {x: 1.1; y: 6}
            XYPoint {x: 0.8; y: 6}
            XYPoint {x: 0.8; y: 10}
            XYPoint {x: 0.6; y: 10}
            XYPoint {x: 0.6; y: 3}
            XYPoint {x: 0.5; y: 3}
        }
        borderColor: "transparent"
    }
    AreaSeries{
        axisX: axisX
        axisY: axisY
        color: "gray"
        opacity: 0.6
        upperSeries: LineSeries{
            id: mask
            XYPoint {x: 0.3; y: axisY.max}
            XYPoint {x: 0.5; y: axisY.max}
        }
        borderColor: "transparent"
    }
    LineSeries{
        id: maxthreshold
        property bool focused: false
        property double x: 0.3
        axisX: axisX
        axisY: axisY
        color: "#1f9b98"
        opacity: 0.6
        width: 2
        style: Qt.DotLine
        XYPoint {x: 0.3; y: 0}
        XYPoint {x: 0.3; y: axisY.max}

        onHovered: {
            focused = !focused
            if (focused){
                msarea.cursorShape = Qt.PointingHandCursor
                msarea.selected = maxthreshold
                color = "lightgreen"
            }else{
                msarea.cursorShape = Qt.ArrowCursor
                msarea.selected = undefined
                color = "#1f9b98"
            }
        }

        function setX(aX){
            x = aX
            clear()
            append(x, 0)
            append(x, axisY.max)
            mask.clear()
            mask.append(x, axisY.max)
            mask.append(minthreshold.x, axisY.max)
        }

    }
    LineSeries{
        id: minthreshold
        property bool focused: false
        property double x: 0.5
        axisX: axisX
        axisY: axisY
        color: "#9e0a0a"
        opacity: 0.6
        width: 2
        style: Qt.DotLine
        XYPoint {x: 0.5; y: 0}
        XYPoint {x: 0.5; y: axisY.max}

        onHovered: {
            focused = !focused
            if (focused){
                msarea.cursorShape = Qt.PointingHandCursor
                msarea.selected = minthreshold
                color = "pink"
            }else{
                msarea.cursorShape = Qt.ArrowCursor
                msarea.selected = undefined
                color = "#9e0a0a"
            }
        }
        function setX(aX){
            x = aX
            clear()
            append(x, 0)
            append(x, axisY.max)
            mask.append(maxthreshold.x, axisY.max)
            mask.append(x, axisY.max)
        }
    }
    MouseArea{
        id: msarea
        property var selected
        anchors.fill: parent
        onPositionChanged: {
            //console.log(selected)
            //console.log(mouseX)
            var tmp = parent.mapToValue(Qt.point(mouseX, mouseY))
            if (selected === maxthreshold || selected === minthreshold)
                if (oneThreshold){
                    if (tmp.x >= 0 && tmp.x <= 1){
                        minthreshold.setX(tmp.x)
                        maxthreshold.setX(tmp.x)
                        drawHistoGram()
                        thresholdChanged(maxthreshold.x, minthreshold.x)
                    }
                }else if (selected === maxthreshold){
                    if (tmp.x >= 0 && tmp.x < minthreshold.x){
                        selected.setX(tmp.x)
                        drawHistoGram()
                        thresholdChanged(maxthreshold.x, minthreshold.x)
                    }
                }else if (selected === minthreshold){
                    if (tmp.x <= 1 && tmp.x > maxthreshold.x){
                        selected.setX(tmp.x)
                        drawHistoGram()
                        thresholdChanged(maxthreshold.x, minthreshold.x)
                    }
                }
        }
    }

    function clear(){
        goodarea.clear()
        interarea.clear()
        badarea.clear()
    }

    function drawHistoGram(aHistogram){
        if (aHistogram !== undefined)
            histogram = aHistogram
        if (!histogram || histogram.length == 0)
            return
        var interval = 1 / histogram.length
        /*var histogram = {}
        for (var i in scores){
            if (scores[i] === null)
                continue
            var k = Math.round(scores[i] / interval)
            if (k * interval >= 1)
                k--
            if (histogram[k] === undefined)
                histogram[k] = 0
            histogram[k]++;
        }*/

        clear()
        var mx = 0;
        for (var j in histogram){
            if (histogram[j] > mx)
                mx = histogram[j]
            var lft = parseInt(j) * interval
            var rt = (parseInt(j) + 1) * interval
            //console.log(rt)
            var area
            if (rt < maxthreshold.x){
                area = goodarea
            }else if (lft < minthreshold.x){
                area = interarea
            }else{
                area = badarea
            }
            area.append(lft, 0)
            area.append(lft, histogram[j])
            area.append(rt, histogram[j])
            area.append(rt, 0);
        }
        axisY.max = mx + 1
        minthreshold.setX(minthreshold.x)
        maxthreshold.setX(maxthreshold.x)
    }
    Component.onCompleted: {
        clear()
        if (oneThreshold)
            maxthreshold.setX(minthreshold.x)
    }
}
