# Abstract
a matrix component for qml  

# Attribute
* rowcap: the row title Label  
* colcap: the column title Label  
* content: the matrix data, it is a 2D array  
* signal selected: emit the signal when the item is selected  
_sample_:  
```
Matrix{
    rowcap.text: "hello"
    colcap.text: "world"
    content: [[1, 2], [3, 4], [5, 6]]
}
```  
</br>

# Test and Demo
test.qml: qsTr("matrix")  
</br>