# Abstract
* the color dialog for qml  

# Attribute
* name: QString: the instance name  
</br>

# API Pipe
* **name + _selectColor**  
    - open color dialog and select color  
    - type: pipeDelegate : name + _colorSelected  
    - pipeline: qml; trigger  
    - input: any  
_sample_:  
```
Pipeline.run("_selectColor", {}, "manual2", {hello: "world"})
```  
</br>

* **name + _colorSelected**  
    - output the selected color  
    - type: pipePartial  
    - pipeline: qml; listener  
    - ouput: string  
</br>