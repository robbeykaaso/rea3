# Abstract
execute the function in a thread pool with 8 threads
```
    aInput -> pipeParallel(threadPool) -> output
    aInput -> pipeParallel(threadPool) -> output
```

# Sample
```
pipeline::instance()->add<QString, pipeParallel>([](rea::stream<QString>* aInput){
    aInput->out();
}, Json("delegate", "pipe"))  //if the function is null, it will use the function of the delegate in the same pipeline, else it will use the function
->next("doSomething");
```  
</br>

# Reference
[pipe](pipe.md)