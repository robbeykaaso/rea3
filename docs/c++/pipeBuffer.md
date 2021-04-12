# Abstract
next pipes will not be executed until this pipe collects the specific count data
```
    aInput -> pipeBuffer(2)
    aInput -> pipeBuffer(2) -> output
```  

# API
* **pipe0* buffer(const int aCount = 1, const QString& aName = "", const int aThread = 0)**
    - create a pipeBuffer  
    - `aCount` is the buffer count  
    - `aName` and `aThread` is the pipe name and the work thread  
</br>

# Sample
**add and connect pipe:**  
```
pipeline::add<int, pipeBuffer>(nullptr, dst::Json("count", 2))  //c++
->next<std::vector<int>>([](stream<std::vector<int>>* aInput){
    aInput->out();
})

Pipeline2.add(null, {name: "test7_", type: "Buffer", count: 2})  //qml
.next(function(aInput){
    var dt = aInput.data()
    console.assert(dt["0"]["hello"] === "world")
    console.assert(dt["00"]["hello"] === "world2")
    aInput.outs("Pass: test7_")
})
```  
</br>

# Test and Demo
test_rea.cpp: test7()  
</br>

# Reference
[pipe](pipe.md)