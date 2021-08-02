# Abstract
* execute the function in a thread pool with 8 threads  
* inherit from `pipePartial`  
```
    aInput -> pipeParallel(threadPool) -> output
    aInput -> pipeParallel(threadPool) -> output
```

# Sample
```
pipelines().add(lambda: aInput:
    aInput->out(), {"delegate": "pipe", "type": "Parallel"})  //if the function is null, it will use the function of the delegate in the same pipeline, else it will use the function
->next("doSomething");
```  
</br>

# Reference
[pipe](../pipe.md)