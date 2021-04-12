# Abstract
the progress window for qml  

# API Pipe
* **updateProgress**  
    - update progress window and output the calculated double progress. the progress window will opened when the progress is not 1  
    - input: QJsonObject  
    - output: number  
    - type: pipePartial  
_sample_:  
```
Pipeline2.run("updateProgress", {title: "demo: ", sum: 10}, "service1")  //initialize the progress window
Pipeline2.run("updateProgress", {step: 2}, "service1")  //update the progress, if there is no "step", the default is 1
```  
</br>

# Test and Demo
test.qml: qsTr("progress")  
</br>