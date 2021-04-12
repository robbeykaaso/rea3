# Abstract
* a container and interface for passing data between pipes  
</br>

# API
* **T data()**  
    - return self data  
</br>

* **stream<T\>* setData(T aData)**  
    - set self data  
    - return self  
</br>

* **stream<T\>* out(const QString& aTag = "")**  
    - pass the stream to all its next pipes by default  
    - `aTag` decides which pipe of nexts will be executed if this is a pipePartial  
    - return self  
_example_:  
```
    out(); // pass the stream to all its next pipes and run them by alphabet sequence
```  
</br>

* **stream<S\>* outs(S aOut = S(), const QString& aNext = "", const QString& aTag = "", bool aShareCache = true)**  
    - pass the stream to its next specific pipe by `aNext`  
    - if `aNext` equals to "", it will only run all the nexts  
    - if `aNext` doesn't equal to "", it will try to run the `localPipe` with the same name. if there still not exists the same name `localPipe`, it will try to run the specific pipe directly on whole range  
    - this function will format the stragety of `out`  
    - `aShareCache` denotes whether the out stream to share the scope cache with self  
    - return this out stream  
_example_:
```
    out<int>(5, "pipe1");  // pass the stream to pipe1 and run pipe1 on next
    out<int>(5, "");  // pass the stream to all next the pipes whose name is "" and run them
```  
</br>

* **stream<S\>* outsB(S aOut = S(), const QString& aNext = "", const QString& aTag = "", bool aShareCache = true)**  
    - work like `outs`  
    - return this stream  
</br>

* **stream<S\>* outs(S aOut, const QString& aNext, const QString& aTag, int aShareCache)**  
    - work as the `outs` above  
    - `aShareCache` is to use the specific scopecache of the existed cache, the index out of range will use self scopecache  
</br>

* **stream<S\>* outsB(S aOut, const QString& aNext, const QString& aTag, int aShareCache)**  
    - work like `outs`  
    - return this stream  
</br>

* **noOut()**  
    - clear the out streams flowing the next pipes  
</br>

* **stream<T\>* var(const QString& aName, S aData)**  
    - cache some data by name  
    - if `aName` has been existed in the scope cache, it will replace the old cache  
    - return this stream  
</br>

* **S varData(const QString& aName)**  
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

* **QString cache(const QString& aID = "")**  
    - cache the stream into the pipeline  
    - it is used with `runC` of pipeline for the continous of the pipeDelegate  
    - it returns the stream id  
    - `aID` is provoided for other languages wrapper  
</br>

* **QString transactionName()**  
    - return the current transaction name  
</br>

* **std::shared_ptr<stream<S\>\> map(S aInput = S())**
    - return the stream of the input data  
</br>

* **std::shared_ptr<stream<S\>\> call(const QString& aName)**  
    - asynchronously execute the pipe function  
    - return the result stream of the function  
    - notice: ensure the function has output streams  
_sample_:  
```
    aInput->map<QJsonArray>()->call("project_label_listViewSelected")
```
</br>

* **std::shared_ptr<stream<S\>\> call(pipeFunc<T> aFunc, const QJsonObject& aParam = QJsonObject())**  
    - asynchronously execute the lamda function  
    - `aParam` is the param of this pipe  
    - return the result stream of the function  
    - notice: ensure the function has output streams
</br>