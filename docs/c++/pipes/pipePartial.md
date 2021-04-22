# Abstract
* only execute the next specific pipe which satisfies some condition  
```
//we will only need execute pipe3 after pipePartial sometimes rather than pipe4
             -> pipe3  
pipePartial
             x-> pipe4
```
</br>

# Sample
**add and connect pipe:**  
```
pipeline::instance()->add<QJsonObject, pipePartial>([](stream<QJsonObject>* aInput){
    aInput.out();
}, Json("name", "WCS2SCS"))
->next("doSomething", "service1")
```  

**make it work:**  
```
pipeline::instance()->run<QJsonObject>("WCS2SCS", QJsonObject(), "service1")  //the conection with service1 tag will be executed
```  
</br>

# Reference
[pipe](../pipe.md)