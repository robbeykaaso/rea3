# Abstract
next pipes will be executed by its delegate pipe
```
    input -> pipeDelegate                        
                   -> delegate pipe -> output
```

# Sample
**add and connect pipe:**  
```
re::pipeManager::add<QJsonObject, pipeDelegate>(stream<QJsonObject>* aInput){
    
}, dst::Json("name", "selectFolder", "delegate", "folderSelected"))
->next("doSomething")  //selectFolder -> doSomething  ==> folderSelected -> doSomething

re::pipeManager::add<QJsonObject>([](stream<QJsonObject>* aInput){
    aInput.out()
}, dst::Json("name", "folderSelected"));
```  
</br>

# Test and Demo
test_rea.cpp: test5()  
</br>

# Reference
[pipe](pipe.md)