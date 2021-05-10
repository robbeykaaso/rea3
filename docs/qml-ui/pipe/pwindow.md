# Abstract
* the wrapper of PWindow0  

# API Pipe
* **_setParam**
    - generate gui by model  
    - type: pipeDelegate: _paramSet  
    - pipeline: qml; trigger  
    - input: QJsonObject  
</br>  

* **_paramSet**
    - output the editted value  
    - type: pipePartial  
    - pipeline: qml; listenner  
    - output: QJsonObject  
</br>