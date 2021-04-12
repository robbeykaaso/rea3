# Abstract
it will filter the redundant streams between the executing next pipe and the last trig
```
    aInput -> pipeThrottle -> busy
    aInput -> pipeThrottle -> busy
    aInput -> pipeThrottle -> busy //format the last trigged stream, the second stream will be ignored in this case
```

# Sample
**add and connect pipe:**  
```
re::pipeManager::add<QJsonObject, pipeThrottle>(stream<QJsonObject>* aInput){
    
})
->next("doSomething")
```  
</br>

# Test and Demo
test_rea.cpp: test10()  
</br>

# Reference
[pipe](pipe.md)