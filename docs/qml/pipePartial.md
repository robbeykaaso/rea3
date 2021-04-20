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
Pipeline.add(function(aInput){
    aInput.out()
}, {name: "WCS2SCS", type: "Partial"})
->next("doSomething", "service1")
```  

**make it work:**  
```
Pipeline.run("WCS2SCS", {}, "service1")  //the conection with service1 tag will be executed
```  
</br>

# Reference
[pipe](pipe.md)