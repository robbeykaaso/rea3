# Abstract
* a step in the pipeline  
* a stream is passed by many pipes and each pipe may do one thing on one thread, such as  
```
input -> pipe1(calculate on thread2) -> pipe2(store data on thread1) -> pipe3(render GUI on thread0) -> output
```  
</br>

# API
* **actName(self) -> str**  
    - `return` the actual name of pipe  
</br>

* **next(self, aName: str, aTag: str = "") -> 'pipe'**  
    - connect a pipe after self  
    - `aName` is the next pipe name
    - `aTag` is the pipe connected parameter  
    - `return` the next pipe  
_sample_:
```
next("doSomething", "tag0;tag1")  //different tags could be split by semicolon
```  
</br>

* **nextP(self, aNext: 'pipe', aTag: str = "") -> 'pipe'**  
    - work like `next()`  
    - `aNext` is the next pipe  
</br>

* **nextF(self, aFunc, aTag: str = "", aParam: dict = {}) -> 'pipe'**  
    - work like `next()`  
    - `aFunc` is the next pipe function  
    - `aParam` is the next pipe parameter  
</br>

* **nextB(self, aName: str, aTag: str = "") -> 'pipe'**  
    - work like `next()`  
    - `return` self  
</br>

* **nextPB(self, aNext: 'pipe', aTag: str = "") -> 'pipe'**  
    - work like `nextP()`  
    - `return` self  
</br>

* **nextFB(self, aFunc, aTag: str = "", aParam: dict = {}) -> 'pipe'**  
    - work like `nextB()`  
    - `return` self  
</br>

* **removeNext(self, aName: str, aAndDelete: bool = False, aOutside: bool = True)**  
    - remove the next pipe  
    - `aName` is the next pipe name  
    - `aAndDelete` is whether to delete the next pipe  
    - `aOutside` is whether to remove the same name pipe outside of this pipelines  
</br>

* **removeAspect(self, aType: str, aAspect: str = None)**  
    - remove the aspect of the self  
    - `aType` contains `before`, `around` and `after`  
    - `aAspect` is the aspect pipe name. it will remove all this kind of aspects if it is ""  
</br>

# Related pipes
_pipeFuture, [pipeDelegate](pipes/pipeDelegate.md), [pipePartial](pipes/pipePartial.md), [pipeAsync](pipes/pipeAsync.md), [pipeParallel](pipes/pipeParallel.md)..._  

# Notice  
- <font color="red">pipe in python is not thread safe if you dynamically remove or use pipefuture to trig remove, please use these operations carefully</font><br />  
</br>