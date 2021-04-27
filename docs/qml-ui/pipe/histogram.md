# Abstract
* the wrapper of histogram0    

# Attribute
* name: QString: the instance name  
</br>

# API Pipe
* **name + _updateHistogramGUI**  
    - update the histogram show  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonObject  
    - output: QJsonObject:  
        - histogram: QJsonArray : each column value  
_sample_:  
```
Pipeline.run("_updateHistogramGUI", {histogram: [40, 20, 15, 25, 14, 16, 13, 30]})
```  
</br>

# Test and Demo
test.qml: qsTr("histogram")  
</br>