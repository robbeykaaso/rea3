# Abstract
* manage self pipes' life  
* the stream data supports types of `string`, `number`, `bool`, `array`, `object` and the customized type in rea-c++  

# API
* **add(aFunc, aParam = {})**  
    - add a pipe of the specific type into the pipeline  
    - `aFunc` is the function to be executed  
    - `aParam` is the pipe parameter  
    - `return` the specific pipe  
_sample_:  
```
pipelines().add(function(aInput){
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

topo result: pipe0' = pipe4 -> pipe0 -> pipe5
             pipe... -> pipe0' -> pipe1 -> pipe...
             pipe... -> pipe2 -> pipe0' -> pipe...
             pipe... -> pipe3(replaced by pipe0') ->pipe...
```  
* Notice  
    - <font color="red">aop attributes include `before`, `after`, `around`, `befored`, and `aftered` could only work in the same pipeline</font><br />  
    - <font color="red">if there is a pipe with the same name, the original pipe will be removed</font><br />  
    - <font color="red">the `external` pipe's `next` could only be modified by the specific pipeline</font><br />  
</br>

* **find(aName, aNeedFuture = true)**  
    - find the specific pipe by name  
    - `aName` is the pipe name  
    - `aNeedFuture` is used for the case that the pipe is not existed now, but will be existed in the future or other pipelines  
    - `return` the pipe  
</br>

* **run(aName, aInput, aTag = "", aScope = null)**  
    - execute the specific pipe by name and start a business procedure  
    - `aName` is the pipe name  
    - `aInput` is the input stream data  
    - `aTag` is the tag name of the stream  
    - `aScope` is the stream additional data  
    - `return` the input stream  
_sample_:
```
pipelines().run("pathSelected", {path: ""}, "service1")
```  
</br>

* **call(aName, aInput, aScope = null)**  
    - only execute the specific pipe and its aspects synchronously  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `aScope` is the stream additional data  
    - `return` the input stream  
</br>

* **input(aInput, aTag = "", aScope = null, aAutoTag = false)**  
    - generate a stream by the input data  
    - `aInput` is the input data  
    - `aTag` is the tag name of the stream  
    - `aScopeCache` is the stream additional data  
    - `aAutoTag` denotes whether to auto generating the tag if the input tag is ""  
    - `return` the generated stream  
</br>

* **remove(aName, aOutside = false)**  
    - remove the specific pipe in this pipeline or in all pipelines  
    - `aName` is the pipe name  
    - `aOutside` is whether to remove the samed name pipe of all the pipelines  
</br>

* **name()**  
    - `return` the pipeline name  
</br>