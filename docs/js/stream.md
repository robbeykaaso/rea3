# Abstract  
* a data container between pipes  

# API
* **data()**  
    - `return` stream data  
</br>

* **setData(aData)**  
    - `aData` is the stream data  
    - `return` self  
</br>

* **out(aTag = "")**  
    - let the stream flowing to all its next pipes  
    - `aTag` is to set the stream tag if it is not ""  
    - `return` self  
* Notice  
    - <font color="red">if the `outs()` is not used, the executing sequence of next pipes will be by alphabet sequence</font><br />  
</br>

* **outs(aOut, aNext = "", aTag = "")**  
    - let the stream flowing to the next specific pipe  
    - `aOut` is the outstream data  
    - `aNext` is the next pipe name  
    - `aTag` is the outstream tag  
    - `return` the outstream  
* Notice  
    - <font color="red">if `aNext` equals to "", it will only execute all the next pipes like `out()`</font><br />  
    - <font color="red">if `aNext` doesn't equal to "", it will try to execute the specific next pipe</font><br />  
</br>

* **outsB(aOut, aNext = "", aTag = "")**  
    - work like `outs()`  
    - `return` self  
</br>

* **void noOut()**  
    - clear the out streams flowing the next pipes  
</br>

* **async asyncCall(aName, aPipeline = pipelines())**  
    - execute the specific pipe asynchronously  
    - `aName` is the pipe name  
    - `aPipeline` is the specific pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **async asyncCallF(aFunc, aParam = {}, aPipeline = pipelines())**  
    - execute the function asynchronously  
    - `aFunc` is the function  
    - `aParam` is the pipe param  
    - `aPipeline` is the specific pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **async asyncCallS()**  
    - execute the pipes and functions sequentially  
_sample_:
```
    pipelines().input(0).asyncCallS([
        function(){

        },
        "doSomething",
        function(){

        }
    ])
```
</br>

* **scope(aNew = false)**  
    - `aNew` is whether to create a new scope cache  
    - `return` the scope cache  
</br>

* **tag()**  
    - `return` the stream tag  
</br>