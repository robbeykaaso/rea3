# Abstract
* next pipes will be executed by its delegate pipe
```
    input -> pipeDelegate                        
                   -> delegate pipe -> output
```

# Sample
```
pipelines().add(lambda: aInput:
    aInput.out(), {"name": "selectFolder", "delegate": "folderSelected", "type": "Delegate"}) \
.next("doSomething")  //selectFolder -> doSomething  ==> folderSelected -> doSomething

pipelines().add(lambda: aInput:
    aInput.out(), {"name": "folderSelected"})
```  
</br>

# Reference
[pipe](../pipe.md)