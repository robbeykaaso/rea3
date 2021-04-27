# Abstract
* a matrix component for qml  

# Attribute
* rowcap: Label object: the row title  
* colcap: Label object: the column title  
* content: QJsonArray: the matrix data  
    - QJsonArray: each line data  
* selected: signal: it is emitted when the item is selected  
_sample_:  
```
Matrix{
    rowcap.text: "hello"
    colcap.text: "world"
    content: [[1, 2], [3, 4], [5, 6]]
}
```  
</br>