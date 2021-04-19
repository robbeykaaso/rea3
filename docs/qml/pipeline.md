# Abstract
* a singleton class which provide the same APIs like pipeline for QML  
* the stream in QML only support types of `string`, `number`, `bool`, `array` and `object`  

# API
* **QVariant add(QJSValue aFunc, const QJsonObject& aPipeParam = QJsonObject())**  
    - add a pipe into the line  
    - return the qmlPipe of the specific type  
_sample_:  
```
import Pipeline 1.0
Pipeline.add(function(aInput){
    aInput.out()
})
```  
</br>

* **QVariant find(const QString& aName)**  
    - find the specific qmlPipe by name  
</br>

* **void run(const QString& aName, const QJSValue& aInput, const QString& aTag = "", bool aTransaction = true, const QJsonObject& aScopeCache = QJsonObject())**  
    - execute the specific pipe by name  
    - `aTransaction` denotes whether to create atransaction for this whole procedure  
    - `aScopeCache` denotes the scopecache for the stream  
_sample_:
```
Pipeline.run("pathSelected", {path: ""}, "service1")
```  
</br>

* **void runC(const QString& aName, const QJSValue& aInput, const QString& aStreamID, const QString& aTag = "")**  
    - execute the specific pipe by name with the cached stream  
    - it is used with `cache` of stream for pipeDelegate  
</br>

* **void syncCall<T, F\>(const QString& aName, const QJSValue& aInput)**  
    - only execute the specific pipe synchronously on current thread  
</br>

* **QVariant call(const QString& aName, const QJSValue& aInput)**  
    - only execute the specific pipe asynchronously  
    - return the result stream  
    - notice: it will create a qml object and the garbage collection of qml will delete it not instantly like smarter pointer in c++  
</br>

* **QVariant input(const QJSValue& aInput, const QString& aTag = "", bool aTransaction = true, const QJsonObject& aScopeCache = QJsonObject());**  
    - return the stream of the input  
    - `aTransaction` denotes whether to create atransaction for this whole procedure  
    - `aScopeCache` denotes the scopecache for the stream  
    - notice: it will create a qml object and the garbage collection of qml will delete it not instantly like smarter pointer in c++  
</br>

* **void remove(const QString& aName)**  
    - remove the specific pipe by name  
</br>

* **void removeAspect(const QString& aPipe, pipe0::AspectType aType, const QString& aAspect = "")**  
    - remove the aspect of the pipe  
    - `aType` is the aspect type, 0 is before, 1 is around, 2 is after  
    - if `aAspect` is nothing, all the aspects of this type will be removed  
</br>

* **QVariant tr(const QString& aOrigin)**  
    - translate the string to the target string in run time  
</br>

# Test and Demo
test_rea.cpp: test8()