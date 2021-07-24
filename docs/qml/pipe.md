# Abstract
* a step in the pipeline  
* a stream is passed by many pipes and each pipe may do one thing on one thread  

# API
* **QString actName()**  
    - `return` the actual name of pipe  
</br>

* **QJSValue next(const QString& aName, const QString& aTag = "")**  
    - connect a pipe after self  
    - `aName` is the next pipe name
    - `aTag` is the pipe connected parameter  
    - `return` the next pipe  
_sample_:
```
    import Pipeline 1.0
    Pipeline.find("selectPath")
            .next("doSomething", "service1")
```  
</br>

* **QJSValue nextF(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject())**  
    - work like `next()`  
    - `aFunc` is the next pipe function  
    - `aParam` is the next pipe parameter  
_sample_:
```
    import Pipeline 1.0
    Pipeline.find("selectPath")
            .next(function(aInput){
                aInput.out()
            }, "service1", {name: "doSomething"})
```  
</br>

* **QJSValue nextB(const QString& aName, const QString& aTag = "")**  
    - work like `next()`  
    - `return` self  
</br>

* **QJSValue nextFB(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject())**  
    - work like `nextB()`  
    - `return` self  
</br>

* **void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true)**  
    - remove the next pipe  
    - `aName` is the next pipe name  
    - `aAndDelete` is whether to delete the next pipe  
    - `aOutside` is whether to remove the same name pipe outside of this pipelines  
</br>

* **void removeAspect(const QString& aType, const QString& aAspect = "")**
    - remove the aspect of the self  
    - `aType` contains "before", "around" and "after"  
    - `aAspect` is the aspect pipe name. it will remove all this kind of aspects if it is ""  
</br>

# Related pipes
_[pipeDelegate](pipeDelegate.md), [pipePartial](pipePartial.md)_