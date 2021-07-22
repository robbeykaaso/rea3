# Abstract
* a step in the pipeline  
* a stream is passed by many pipes and each pipe may do one thing on one thread, such as  
```
input -> pipe1(calculate on thread2) -> pipe2(store data on thread1) -> pipe3(render GUI on thread0) -> output
```  
</br>

# API
* **QString actName()**  
    - `return` the actual name of pipe  
</br>

* **pipe0* next(const QString& aName, const QString& aTag = "")**  
    - connect a pipe after self  
    - `aName` is the next pipe name
    - `aTag` is the pipe connected parameter  
    - `return` the next pipe  
_sample_:
```
next("doSomething", "tag0;tag1")  //different tags could be split by semicolon
```  
</br>

* **pipe0* nextP(pipe0* aNext, const QString& aTag = "")**  
    - work like `next()`  
    - `aNext` is the next pipe  
</br>

* **pipe0* nextF<T\>(pipeFunc<T> aNextFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject())**  
    - work like `next()`  
    - `aNextFunc` is the next pipe function  
    - `aParam` is the next pipe parameter  
</br>

* **pipe0* nextB(const QString& aName, const QString& aTag = "")**  
    - work like `next()`  
    - `return` self  
</br>

* **pipe0* nextPB(pipe0* aNext, const QString& aTag = "")**  
    - work like `nextP()`  
    - `return` self  
</br>

* **pipe0* nextFB<T\>(pipeFunc<T> aNextFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject())**  
    - work like `nextB()`  
    - `return` self  
</br>

* **void removeNext(const QString& aName)**  
    - remove the next pipe  
    - `aName` is the next pipe name  
</br>

* **void removeAspect(pipe0::AspectType aType, const QString& aAspect = "")**  
    - remove the aspect of the self  
    - `aType` contains `AspectBefore`, `AspectAround` and `AspectAfter`  
    - `aAspect` is the aspect pipe name. it will remove all this kind of aspects if it is ""  
</br>

# Related pipes
_pipeFuture, [pipeDelegate](pipes/pipeDelegate.md), [pipePartial](pipes/pipePartial.md), [pipeAsync](pipes/pipeAsync.md), [pipeParallel](pipes/pipeParallel.md)..._  

# Notice  
- <font color="red">pipe in c++ is not thread safe if you dynamically remove or use pipefuture to trig remove, please use these operations carefully</font><br />  
</br>