# Abstract
* a step in the pipeline  
* a stream is passed by many pipes and each pipe may do one thing on one thread  

# API
* **actName()**  
    - `return` the actual name of pipe  
</br>

* **next(aName, aTag = "")**  
    - connect a pipe after self  
    - `aName` is the next pipe name
    - `aTag` is the pipe connected parameter  
    - `return` the next pipe  
_sample_:
```
    pipelines().find("selectPath")
            .next("doSomething", "service1")
```  
</br>

* **nextF(aFunc, aTag = "", aParam = {})**  
    - work like `next()`  
    - `aFunc` is the next pipe function  
    - `aParam` is the next pipe parameter  
_sample_:
```
    pipelines().find("selectPath")
            .next(function(aInput){
                aInput.out()
            }, "service1", {name: "doSomething"})
```  
</br>

* **nextB(aName, aTag = "")**  
    - work like `next()`  
    - `return` self  
</br>

* **nextFB(aFunc, aTag = "", aParam = {})**  
    - work like `nextB()`  
    - `return` self  
</br>

* **removeNext(aName, bool aAndDelete = false, bool aOutside = true)**  
    - remove the next pipe  
    - `aName` is the next pipe name  
    - `aAndDelete` is whether to delete the next pipe  
    - `aOutside` is whether to remove the same name pipe outside of this pipelines  
</br>

# Related pipes
_[pipeDelegate](pipeDelegate.md), [pipePartial](pipePartial.md)_