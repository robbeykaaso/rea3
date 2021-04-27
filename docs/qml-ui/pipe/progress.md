# Abstract
* the progress window for qml  

# API Pipe
* **updateProgress**  
    - update progress window and output the calculated double progress. the progress window will opened when the progress is not 1  
    - type: pipePartial  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - title: QString: window title  
        - sum: int: the whole progress count  
        - step: int: the progress updating step; default step is 1; negative step will increase the `sum`  
    - output: double: progress value  
_sample_:  
```
Pipeline.run("updateProgress", {title: "demo: ", sum: 10}, "service1")  //initialize the progress window
Pipeline.run("updateProgress", {step: 2}, "service1")  //update the progress, if there is no "step", the default is 1
```  
</br>