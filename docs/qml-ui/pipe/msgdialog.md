# Abstract
the message dialog component for qml  

# API Pipe
* **popMessage**  
    - popup the message dialog and show message  
    - input: QJsonObject  
    - type: pipeDelegate: messagePoped  
_sample_:  
```
Pipeline2.run("popMessage", {title: "hello4", text: "world"}, "service1")
```  
</br>

* **messagePoped**  
    - output the result `ok`  
    - output: QJsonObject  
    - type: pipePartial  
</br>

# Test and Demo
test.qml: qsTr("MsgDialog")  
</br>