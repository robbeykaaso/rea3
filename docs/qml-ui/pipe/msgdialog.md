# Abstract
* the message dialog component for qml  

# API Pipe
* **popMessage**  
    - popup the message dialog and show message  
    - type: pipeDelegate: messagePoped  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - title: QString: dialog title  
        - text: QString: dialog content  
_sample_:  
```
Pipeline.run("popMessage", {title: "hello4", text: "world"}, "service1")
```  
</br>

* **messagePoped**  
    - whether is OK  
    - type: pipePartial  
    - pipeline: qml; listener  
    - output: bool  
</br>