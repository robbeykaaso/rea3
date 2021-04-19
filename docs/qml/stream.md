# Abstract  
* the stream class in qml  

# API
* **QJSValue data()**  
    - return self data  
</br>

* **QVariant setData(QJSValue aData)**  
    - set self data  
    - return self  
</br>

* **QVariant out(const QString& aTag = "")**  
    - pass the stream to all its next pipes by default  
    - `aTag` decides which pipe of nexts will be executed if this is a pipePartial  
    - return self  
</br>

* **QVariant outs(QJSValue aOut, const QString& aNext = "", const QString& aTag = "", bool aShareCache = true)**  
    - pass the stream to its next specific pipe by `aNext`  
    - if `aNext` equals to "", it will only run all the nexts  
    - if `aNext` doesn't equal to "", it will try to run the `localPipe` with the same name. if there still not exists the same name `localPipe`, it will try to run the specific pipe directly on whole range  
    - this function will format the stragety of `out`  
    - `aShareCache` denotes whether the out stream to share the scope cache with self  
    - return this out stream  
</br>

* **QVariant outsB(QJSValue aOut, const QString& aNext = "", const QString& aTag = "", bool aShareCache = true)**  
    - work like `outs`  
    - return this stream  
</br>

* **stream<S\>* outs(QJSValue aOut, const QString& aNext, const QString& aTag, int aShareCache)**  
    - work as the `outs` above  
    - `aShareCache` is to use the specific scopecache of the existed cache, the index out of range will use self scopecache  
</br>

* **stream<S\>* outsB(QJSValue aOut, const QString& aNext, const QString& aTag, int aShareCache)**  
    - work like `outs`  
    - return this stream  
</br>

* **noOut()**  
    - clear the out streams flowing the next pipes  
</br>

* **QVariant var(const QString& aName, QJSValue aData)**  
    - cache some data by name  
    - if `aName` has been existed in the scope cache, it will replace the old cache  
    - return this stream  
</br>

* **QJSValue varData(const QString& aName, const QString& aType = "object")**  
    - `aType` is the type of the variable
    - return the specific cache data by its name  
</br>

* **QString tag()**  
    - return the tag of this stream  
</br>

* **void fail()**  
    - tag fail status for this transaction if exists  
</br>

* **bool failed()**  
    - return whether the transaction is failed  
</br>

* **void log(const QString& aLog)**  
    - add a log to this transaction if exists  
</br>

* **QString cache()**  
    - cache the stream into the pipeline  
    - it is used with `runC` of pipeline for the continous of the pipeDelegate  
    - it returns the stream id  
</br>

* **QString transactionName()**  
    - return the current transaction name  
</br>

* **QVariant map(QJSValue aInput)**  
    - return the stream of the input data  
    - notice: it will create a qml object and the garbage collection of qml will delete it not instantly like smarter pointer in c++  
</br>

* **QVariant call(const QString& aName, const QString& aType = "")**  
    - asynchronously execute the pipe function  
    - return the result stream of the function  
    - `aType` is the output type such as `string`, `number`, `bool`, `array` and `object`  
    - notice: ensure the function has output streams  
_sample_:  
```
    aInput.map("testFS2.json").call("readJson2")
```
</br>

* **QVariant call(QJSValue aFunc, const QJsonObject& aPipeParam = QJsonObject())**  
    - asynchronously execute the js function  
    - `aParam` is the param of this pipe  
    - return the result stream of the function  
    - notice: ensure the function has output streams  
</br>