# Abstract
the wrapper of histogram0    

# Attribute
* name: the instance name  
</br>

# API Pipe
* **name + _updateHistogramGUI**  
    - update the histogram show  
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
_sample_:  
```
Pipeline.run("_updateHistogramGUI", {histogram: [40, 20, 15, 25, 14, 16, 13, 30]})
```  
</br>

# Test and Demo
test.qml: qsTr("histogram")  
</br>