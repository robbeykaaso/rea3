# Abstract
* a wrapper of linechart0  

# Attribute
* name: QString: the instance name    
</br>

# API Pipe
* **name + _updateLineChart**  
    - update the linechart show  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonArray : y data  
    - output: input  
_sample_:  
```
    Pipeline.run("_updateLineChart", [20, 30, 100, 125, 30, 10, 12, 30, 50])
```  
</br>