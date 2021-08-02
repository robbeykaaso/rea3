# Abstract
* manage self pipes' life  
* provide the interface to call pipe or run procedure  
</br>

# API
* **add(self, aFunc, aParam: dict = {}) -> pipe**  
    - add a pipe of the specific type into the pipeline  
    - `aFunc` is the function to be executed  
    - `aParam` is the pipe parameter  
    - `return` the pipe of the specific type  
_sample_:
```
def p1(aInput: stream):
    aInput.out()
add(p1, {"name": "pipe0",  //the name of the pipe, if there is no name, it will be regarded as an anonymous pipe
         "type": "Partial", //the default is "", other choices can be such as Partial, Delegate, Async, Parallel
         "thread": 2,  //which thread the pipe will be executed on. if it is 0, it will be executed on the current thread
         "replace": false, //the pipe will reserve the old pipe's next pipes if they are existed
         "before": "pipe1", //inject this pipe before the target pipe, it will be executed on the same thread of the target pipe
         "after": "pipe2", //work like "before"
         "around": "pipe3" //work like "before", replace the function of this pipe
         "befored": "pipe4",  //inject the target pipe before this pipe, the target pipe will be executed on the same thread of this pipe
         "aftered": "pipe5"  //work like "befored"
         "external": "c++"  //decide which pipeline controls its next pipes
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
    - <font color="red">thread 1 is occupied by `asyncCall` on not event level</font><br />  
</br>

* **find(self, aName: str, aNeedFuture: bool = True) -> pipe**  
    - find the specific pipe by name  
    - `aName` is the pipe name  
    - `aNeedFuture` is used for the case that the pipe is not existed now, but will be existed in the future or other pipelines  
    - `return` the pipe  
</br>

* **run(self, aName: str, aInput: any, aTag: str = "", aScope: scopeCache = None) -> stream**  
    - execute the specific pipe by name and start a business procedure  
    - `aName` is the pipe name  
    - `aInput` is the input stream data  
    - `aTag` is the tag name of the stream  
    - `aScope` is the stream additional data  
    - `return` the input stream  
_sample_:  
```
pipelines().run("pipe0", 0, "service0")
```  
</br>

* **call(self, aName: str, aInput: any = 0, aScope: scopeCache = None, aAOP: bool = True) -> stream**  
    - only execute the specific pipe and its aspects synchronously on current thread  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `aScope` is the stream additional data  
    - `aAOP` is to only execute the function self rather than the pipe  
    - `return` the input stream  
</br>

* **input(self, aInput: any, aTag: str = "", aScope: scopeCache = None, aAutoTag: bool = False) -> stream**  
    - generate a stream by the input data  
    - `aInput` is the input data  
    - `aTag` is the tag name of the stream  
    - `aScope` is the stream additional data  
    - `aAutoTag` denotes whether to auto generating the tag if the input tag is ""  
    - `return` the generated stream  
_sample_:  
```
pipelines().input(0)
```
</br>

* **remove(self, aName: str, aOutside: bool = False)**  
    - remove the specific pipe in this pipeline or in all pipelines  
    - `aName` is the pipe name  
    - `aOutside` is whether to remove the samed name pipe of all the pipelines  
</br>

* **name(self) -> str**  
    - `return` the pipeline name  
</br>