# Abstract  
* a data container between pipes  

# API
* **QJSValue data()**  
    - `return` stream data  
</br>

* **QJSValue setData(QJSValue aData)**  
    - `aData` is the stream data  
    - `return` self  
</br>

* **QJSValue out(const QString& aTag = "")**  
    - let the stream flowing to all its next pipes  
    - `aTag` is to set the stream tag if it is not ""  
    - `return` self  
* Notice  
    - <font color="red">if the `outs()` is not used, the executing sequence of next pipes will be by alphabet sequence</font><br />  
</br>

* **QJSValue outs(QJSValue aOut, const QString& aNext = "", const QString& aTag = "")**  
    - let the stream flowing to the next specific pipe  
    - `aOut` is the outstream data  
    - `aNext` is the next pipe name  
    - `aTag` is the outstream tag  
    - `return` the outstream  
* Notice  
    - <font color="red">if `aNext` equals to "", it will only execute all the next pipes like `out()`</font><br />  
    - <font color="red">if `aNext` doesn't equal to "", it will try to execute the specific next pipe</font><br />  
</br>

* **QJSValue outsB(QJSValue aOut, const QString& aNext = "", const QString& aTag = "")**  
    - work like `outs()`  
    - `return` self  
</br>

* **void noOut()**  
    - clear the out streams flowing the next pipes  
</br>

* **QJSValue asyncCall(const QString& aName, bool aEventLevel = true)**  
    - execute the specific pipe asynchronously  
    - `aName` is the pipe name  
    - `aEventLevel` is whether not to block current thread  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **QJSValue asyncCallF(QJSValue aFunc, const QJsonObject& aParam = QJsonObject(), bool aEventLevel = true)**  
    - execute the function asynchronously  
    - `aFunc` is the function  
    - `aParam` is the pipe param  
    - `aEventLevel` is whether not to block current thread  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **QJSValue scope(bool aNew = false)**  
    - `aNew` is whether to create a new scope cache  
    - `return` the scope cache  
</br>

* **QString tag()**  
    - `return` the stream tag  
</br>