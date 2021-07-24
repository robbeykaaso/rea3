# Abstract
* a singleton object which provide the interface to call pipe or run procedure for QML  
* the stream data supports types of `string`, `number`, `bool`, `array`, `object` and the customized type in rea-c++  

# API
* **QJSValue add(QJSValue aFunc, const QJsonObject& aParam = QJsonObject())**  
    - add a pipe of the specific type into the pipeline  
    - `aFunc` is the function to be executed  
    - `aParam` is the pipe parameter  
    - `return` the specific pipe  
_sample_:  
```
import Pipeline 1.0
Pipeline.add(function(aInput){
    aInput.out()
}, {
    "name", "pipe0",  //the name of the pipe, if there is no name, it will be regarded as an anonymous pipe
    "replace", false, //the pipe will reserve the old pipe's next pipes if they are existed
    "before", "pipe1", //inject this pipe before the target pipe, it will be executed on the same thread of the target pipe
    "after", "pipe2", //work like "before"
    "around", "pipe3" //work like "before", replace the function of this pipe
    "befored", "pipe4",  //inject the target pipe before this pipe, the target pipe will be executed on the same thread of this pipe
    "aftered", "pipe5"  //work like "befored"
    "external", "c++"  //decide which pipeline controls its next pipes
})
```  
* Notice  
    - <font color="red">the pipe could only be executed on the main thread</font><br />  
</br>

* **QJSValue find(const QString& aName)**  
    - find the specific pipe by name  
    - `aName` is the pipe name  
    - `return` the pipe  
</br>

* **QJSValue run(const QString& aName, const QJSValue& aInput, const QString& aTag = "", const QJSValue& aScope = QJSValue::NullValue)**  
    - execute the specific pipe by name and start a business procedure  
    - `aName` is the pipe name  
    - `aInput` is the input stream data  
    - `aTag` is the tag name of the stream  
    - `aScope` is the stream additional data : QJsonObject or qmlScopeCache  
    - `return` the input stream  
_sample_:
```
Pipeline.run("pathSelected", {path: ""}, "service1")
```  
</br>

* **QJSValue call(const QString& aName, const QJSValue& aInput, const QJsonObject& aScope = QJsonObject(), bool aAOP = true)**  
    - only execute the specific pipe and its aspects synchronously  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `aScope` is the stream additional data  
    - `aAOP` is to only execute the function self rather than the pipe  
    - `return` the input stream  
</br>

* **QJSValue asyncCall(const QString& aName, const QJSValue& aInput, bool aEventLevel = true)**  
    - only execute the specific pipe and its aspects asynchronously  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `aEventLevel` is whether not to block current thread  
    - `return` the output stream  
</br>

* **QJSValue input(const QJSValue& aInput, const QString& aTag = "", const QJsonObject& aScopeCache = QJsonObject(), bool aAutoTag = false);**  
    - generate a stream by the input data  
    - `aInput` is the input data  
    - `aTag` is the tag name of the stream  
    - `aScopeCache` is the stream additional data  
    - `aAutoTag` denotes whether to auto generating the tag if the input tag is ""  
    - `return` the generated stream  
</br>

* **void remove(const QString& aName, bool aOutside = false)**  
    - remove the specific pipe in this pipeline or in all pipelines  
    - `aName` is the pipe name  
    - `aOutside` is whether to remove the samed name pipe of all the pipelines  
</br>

* **QString tr(const QString& aOrigin)**  
    - translate the string to the target string in run time  
    - `aOrigin` is the original value  
    - `return` the translated value  
</br>