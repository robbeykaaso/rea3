# Abstract
next pipes will be executed by its delegate pipe
```
    input -> pipeDelegate                        
                   -> delegate pipe -> output
```

# Sample
```
pipeline::instance()->add<QJsonObject, pipeDelegate>(stream<QJsonObject>* aInput){
    
}, dst::Json("name", "selectFolder", "delegate", "folderSelected"))
->next("doSomething")  //selectFolder -> doSomething  ==> folderSelected -> doSomething

re::pipeManager::add<QJsonObject>([](stream<QJsonObject>* aInput){
    aInput.out()
}, dst::Json("name", "folderSelected"));
```  
</br>

# Reference
[pipe](pipe.md)