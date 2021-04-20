# Abstract
* next pipes will be executed by its delegate pipe
```
    input -> pipeDelegate                        
                   -> delegate pipe -> output
```

# Sample
```
pipelines().add(function(aInput){
    
}, {name: "selectFolder", delegate: "folderSelected"})
->next("doSomething")  //selectFolder -> doSomething  ==> folderSelected -> doSomething

pipelines().add(function(aInput){
    aInput.out()
}, {name: "folderSelected"})
```  
</br>

# Reference
[pipe](pipe.md)