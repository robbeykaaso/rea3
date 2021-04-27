# Abstract
* the wrapper of matrix0  

# Attribute
* name: QString: the instance name  
</br>

# API Pipe
* **name + _updateMatrix**  
    - update the matrix show  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - rowcap: QString: row title  
        - colcap: QString: column title  
        - content: QJsonArray: maxtrix data  
            - QJsonArray: each row data  
    - output: input  
_sample_:  
```
Pipeline.run("_updateMatrix", { rowcap: "hello2",  //the row title
                                colcap: "world2",  //the column title
                                content: [[1, 2, 3, 4],  //the content
                                          [5, 6, 7, 8],
                                          [9, 10, 11, 12],
                                          [13, 14, 15, 16],
                                          [17, 18, 19, 20]]})
```  
</br>

* **name + _matrixSelected**
    - get the 1D index in the content  
    - type: pipePartial  
    - pipeline: qml; listener  
    - output: double: selected index  
</br>