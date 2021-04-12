# Abstract
it will execute the function in a thread pool with 8 threads
```
    aInput -> pipeParallel(threadPool) -> output
    aInput -> pipeParallel(threadPool) -> output
```

# API
* **pipe0* parallel<T\>(const QString& aName)**  
    - create a pipeParallel of the specific pipe  
_sample_:  
```
parallel("test9");
```  
</br>

# Sample
**add and connect pipe:**  
```
pipeline::add<QString, pipeParallel>([](rea::stream<QString>* aInput){
    aInput->out();
}, Json("delegate", "pipe"))  //if the function is null, it will use the function of the delegate, else it will use the function
->next(local("doSomething"));
```  
</br>

# Test and Demo
test_rea.cpp: test9()  
</br>

# Reference
[pipe](pipe.md)