# Abstract
* a step in the pipeline  
* a stream is passed by many pipes and each pipe may do one thing on one thread, such as  
```
input -> pipe1(calculate on thread2) -> pipe2(store data on thread1) -> pipe3(render GUI on thread0) -> output
```  
</br>

# API
* **QString actName()**  
    - the actual name of pipe  
</br>

* **pipe0* next(const QString& aName / pipe0\* aNext, const QString& aTag = "")**  
    - connect a pipe after self  
    - return the next pipe  
    - the next pipe will try to be executed and get the stream after self is executed  
    - `aTag` is the pipe connected parameter  
_sample_:
```
next("doSomething", "service0")  
```  
</br>

* **pipe0* nextF(pipeFunc<T> aNextFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject())**  
    - `aNextFunc` is the lambda function of this pipe  
    - `aParam` is the param of this pipe  
    - the function is like `next`  
</br>

* **pipe0* nextB(const QString& aName / pipe0\* aNext, const QString& aTag = "")**  
    - connect a next branch pipe  
    - return self  
_sample_:
```
nextB("doSomething", "service0")  
```  
</br>

* **void removeNext(const QString& aName)**  
    - remove the next pipe  
</br>

* **execute(std::shared_ptr<stream0\> aStream)**  
    - trig the pipe and input a stream  
</br>

# Child classes
_pipeFuture, [pipeLocal](pipeLocal.md), [pipeDelegate](pipeDelegate.md), [pipePartial](pipePartial.md), [pipeBuffer](pipeBuffer.md), [pipeThrottle](pipeThrottle.md), [pipeParallel](pipeParallel.md)..._