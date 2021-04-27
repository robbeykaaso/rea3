# Abstract
* the wrapper of log0  

# Attribute
* type: QJsonArray: log types  
* level: QJsonArray: log levels  

# API Pipe
* **addLogRecord**  
    - add one record to the log  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - type: QString: log type  
        - level: QString: level type  
        - msg: QString: record message  
    - output: input  
_sample_:  
```
Pipeline.run("addLogRecord", {type: "train", level: "info", msg: "hello"}) // add the "msg" into ["type"]["level"] loglist
```  
</br>

* **showLogPanel**  
    - set log component "visible" to true  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: any  
    - output: input  
</br>