# Abstract
the wrapper of log0  

# API Pipe
* **addLogRecord**  
    - add the one record to the log  
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
_sample_:  
```
Pipeline.run("addLogRecord", {type: "train", level: "info", msg: "hello"}) // add the "msg" into ["type"]["level"] loglist
```  
</br>

* **showLogPanel**  
    - set log component "visible" to true
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
</br>

# Test and Demo
test.qml: qsTr("log")  
</br>