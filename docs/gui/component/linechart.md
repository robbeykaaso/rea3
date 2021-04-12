# Abstract
a wrapper of linechart0  

# Attribute
* name: the instance name    
</br>

# API Pipe
* **name + _updateLineChart**  
    - update the linechart show
    - input: QJsonArray  
    - output: QJsonArray  
    - type: pipe  
_sample_:  
```
    Pipeline.run("_updateLineChart", [20, 30, 100, 125, 30, 10, 12, 30, 50])
```  
</br>

# Test and Demo
test.qml: qsTr("lineChart")  
</br>