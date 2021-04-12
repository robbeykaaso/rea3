# Abstract
an anonymous pipe which does the same thing as the same named pipe. they use the same callback function, but are different pipe instances

# API
* **pipe0* local(const QString& aName, const QJsonObject& aParam = QJsonObject())**  
    - create a pipeLocal of the specific pipe  
_sample_:  
```
local("test4_0", rea::Json("tag", "service1"));
```  
</br>

# Sample
**add and connect pipe:**  
```
pipeline::add<QString, pipeLocal>(nullptr, Json("name", "pipe0"))
->next(local("doSomething"));

pipeline::add<QString>([](stream<QString>* aInput){
    aInput->out();
}, Json("name", "doSomething"));
```  
</br>

# Test and Demo
test_rea.cpp: test4()  
</br>

# Reference
[pipe](pipe.md)