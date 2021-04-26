# Abstract
the color dialog for qml  

# Attribute
* name: the instance name  
</br>

# API Pipe
* **name + _selectColor**  
    - open color dialog and select color  
    - input: QJsonObject  
    - type: pipeDelegate: name + _colorSelected  
_sample_:  
```
Pipeline.run("_selectColor", {}, "manual2")
```  
</br>

* **name + _colorSelected**  
    - output the selected color  
    - ouput: string  
    - type: pipePartial  
</br>

# Test and Demo
test.qml: qsTr("color")  
</br>