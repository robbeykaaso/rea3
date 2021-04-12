# Abstract
the wrapper of matrix0  

# Attribute
* name: the instance name  
</br>

# API Pipe
* **name + _updateMatrix**  
    - update the matrix show  
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
_sample_:  
```
Pipeline.run("_updateMatrix", {rowcap: "hello2",  //the row title
                                colcap: "world2",  //the column title
                                content: [[1, 2, 3, 4],  //the content
                                          [5, 6, 7, 8],
                                          [9, 10, 11, 12],
                                          [13, 14, 15, 16],
                                          [17, 18, 19, 20]]})
```  
</br>

* **name + _matrixSelected**
    - output the 1D index in the content. its type is PipePartial  
    - output: number  
    - type: pipe  
</br>

# Test and Demo
test.qml: qsTr("matrix")  
</br>