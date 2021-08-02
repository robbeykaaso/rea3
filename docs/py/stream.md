# Abstract
* a data container between pipes  
</br>

# API
* **data(self) -> any**  
    - `return` stream data  
</br>

* **setData(self, aData: any) -> 'stream'**  
    - `aData` is the stream data  
    - `return` self  
</br>

* **out(self, aTag: str = "") -> 'stream'**  
    - let the stream flowing to all its next pipes  
    - `aTag` is to set the stream tag if it is not ""  
    - `return` self  
* Notice  
    - <font color="red">if the `outs()` is not used, the executing sequence of next pipes will be by alphabet sequence</font><br />  
</br>

* **outs(self, aOut: any, aNext: str = "", aTag: str = "") -> 'stream'**  
    - let the stream flowing to the next specific pipe  
    - `aOut` is the outstream data  
    - `aNext` is the next pipe name  
    - `aTag` is the outstream tag  
    - `return` the outstream  
* Notice  
    - <font color="red">if `aNext` equals to "", it will only execute all the next pipes like `out()`</font><br />  
    - <font color="red">if `aNext` doesn't equal to "", it will try to execute the specific next pipe</font><br />  
</br>

* **outsB(self, aOut: any, aNext: str = "", aTag: str = "") -> 'stream'**  
    - work like `outs()`  
    - `return` self  
</br>

* **noOut(self)**  
    - forbid the outflow stream  
</br>

* **asyncCall(self, aName: str, aEventLevel: bool = True, aPipeline: str = "py") -> 'stream'**  
    - execute the specific pipe asynchronously  
    - `aName` is the pipe name  
    - `aEventLevel` is whether not to block current thread  
    - `aPipeline` is the specific pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **asyncCallF(self, aFunc, aParam: dict = {}, aEventLevel: bool = True, aPipeline: str = "py") -> 'stream'**  
    - execute the function asynchronously  
    - `aFunc` is the function  
    - `aParam` is the pipe param  
    - `aEventLevel` is whether not to block current thread  
    - `aPipeline` is the specific pipeline  
    - `return` the result stream  
* Notice  
    - <font color="red">ensure the function has outstreams</font><br />  
</br>

* **scope(self, aNew: bool = False) -> scopeCache**  
    - `aNew` is whether to create a new scope cache  
    - `return` the scope cache  
</br>

* **tag(self) -> str**  
    - `return` the stream tag  
</br>