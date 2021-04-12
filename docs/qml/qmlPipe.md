# Abstract
* a class which wraps the pipe in c++ for QML
* the pipes are all managed in pipeline in c++

# API
* **QString actName()**  
    - the actual name of pipe  
</br>

* **QVariant next(const QString& aName, const QString& aTag = "")**  
    - connect a pipe after this  
    - return the next qmlPipe  
_sample_:
```
    import Pipeline 1.0
    Pipeline.find("selectPath")
            .next("doSomething", "service1")
```  
</br>

* **QVariant next(QJSValue aNext, const QString& aTag = "", const QJsonObject& aPipeParam = QJsonObject())**  
    - connect a pipe after this  
    - return the next qmlPipe  
_sample_:
```
    import Pipeline 1.0
    Pipeline.find("selectPath")
            .next(function(aInput){
                aInput.out()
            }, "service1", {name: "doSomething"})
```  
</br>

* **QVariant nextP(QVariant aNext, const QString& aTag = "")**  
    - connect a pipe after this  
    - return the next uiPipe  
_sample_:
```
    import Pipeline 1.0
    Pipeline.find("selectPath")
            .nextP(Pipeline.add(null, {type: "Buffer", count: 2}))
```  
</br>

* **QVariant nextB(QJSValue aNext, const QString& aTag = "", const QJsonObject& aPipeParam = QJsonObject())**  
    - connect a branch pipe  
    - return self  
</br>

* **QVariant nextB(const QString& aName, const QString& aTag = "")**  
    - connect a branch pipe  
    - return self  
</br>

* **QVariant nextL(const QString& aName, const QString& aTag = "", const QJsonObject& aPipeParam = QJsonObject());**  
    - connect a pipeLocal for the specific pipe  
</br>

* **void removeNext(const QString& aName)**  
    - remove the next pipe  
</br>