# Abstract
* a data container between pipes  
</br>

# API
* **T data()**  
    - `return` stream data  
</br>

* **stream<T\>* setData(T aData)**  
    - `aData` is the stream data  
    - `return` self  
</br>

* **stream<T\>* out(const QString& aTag = "")**  
    - let the stream flowing to all its next pipes  
    - `aTag` is to set the stream tag if it is not ""  
    - `return` self  
* Notice  
    - <font color="red">if the `outs()` is not used, the executing sequence of next pipes will be by alphabet sequence</font><br />  
</br>

* **stream<S\>* outs(S aOut = S(), const QString& aNext = "", const QString& aTag = "")**  
    - let the stream flowing to the next specific pipe  
    - `aOut` is the outstream data  
    - `aNext` is the next pipe name  
    - `aTag` is the outstream tag  
    - `return` the outstream  
* Notice  
    - <font color="red">if `aNext` equals to "", it will only execute all the next pipes like `out()`</font><br />  
    - <font color="red">if `aNext` doesn't equal to "", it will try to execute the specific next pipe</font><br />  
</br>

* **stream<S\>* outsB(S aOut = S(), const QString& aNext = "", const QString& aTag = "")**  
    - work like `outs()`  
    - `return` self  
</br>

* **void noOut()**  
    - forbid the outflow stream  
</br>

* **std::shared_ptr<stream<S\>\> asyncCall<S\>(const QString& aName, bool aEventLevel = true, pipeline* aPipeline = pipeline::instance(), bool aOutside = false)**  
    - execute the specific pipe asynchronously  
    - `aName` is the pipe name  
    - `aEventLevel` is whether not to block current thread  
    - `aPipeline` is the specific pipeline  
    - `aOutside` is whether the pipe is in this pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **std::shared_ptr<stream<S\>\> asyncCallF<S, P\>(pipeFunc<T> aFunc, const QJsonObject& aParam = QJsonObject(), bool aEventLevel = true, pipeline* aPipeline = pipeline::instance())**  
    - execute the function asynchronously  
    - `aFunc` is the function  
    - `aParam` is the pipe param  
    - `aEventLevel` is whether not to block current thread  
    - `aPipeline` is the specific pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **std::shared_ptr<scopeCache\> scope(bool aNew = false)**  
    - `aNew` is whether to create a new scope cache  
    - `return` the scope cache  
</br>

* **QString tag()**  
    - `return` the stream tag  
</br>