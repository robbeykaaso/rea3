# Abstract
only execute the next specific pipe which satisfies some condition  
```
             -> pipe3  //we will only need execute pipe3 after pipePartial sometimes
pipePartial
             x-> pipe4
```
</br>

# Sample
**add and connect pipe:**  
```
pipeline::add<QJsonObject, pipePartial>([](stream<QJsonObject>* aInput){  //c++
    aInput.out();
}, Json("name", "WCS2SCS"))
->next("doSomething", "service1;service2")  //tag the connection; support multiple services by semicolon


Pipeline2.add(function(aInput){  //qml
    aInput.out()  
}, {name: "WCS2SCS", type: "Partial"})
```  

**make it work:**  
```
pipeline::run<QJsonObject>("WCS2SCS", QJsonObject(), "service1")  //the conection with service1 tag will be executed
```  
</br>

# Test and Demo
test_rea.cpp: test6()  
</br>

# Reference
[pipe](pipe.md)