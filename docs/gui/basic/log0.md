# Abstract
the log component for qml  

# Attribute
* type: the default is `["system", "train"]`, it could be overwritten by customized values  
* level: the default is `["info", "warning", "error"]`, it could be overwritten by customized values  
_sample_:
```
    Log{
        type: ["type1", "type2", "type3"]
        level: ["level1", "level2"]
    }
```  
</br>

# Test and Demo
test.qml: qsTr("log")  
</br>