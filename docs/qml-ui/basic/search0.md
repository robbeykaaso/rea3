# Abstract
* the search component for qml  

# Attribute
* text: QString: the input text  
* hint: QString: the placeholder text  
* prefix: QString: the prefix text  
* accepted: signal: it will be emitted when the mouse click or input is complete  
_sample_:  
```
    Search0{
        text: qsTr("hello")
        hint: qsTr("Enter...")
        prefix: "@"
    }
```  
</br>