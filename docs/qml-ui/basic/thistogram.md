# Abstract
* the special histogram with threshold for qml. the x range is scaled in 0 and 1, and the y range depends on the maximum of data  

# Attribute
* histogram: QJsonArray: the histogram data  
* oneThreshold: bool: whether is one thresholds or two thresholds  
* thresholdmax: LineSeries object: the left threshold  
* thresholdmin: LineSeries object: the right threshold  
* thresholdChanged: signal: it will be emitted when the thresholds are changed  
</br>