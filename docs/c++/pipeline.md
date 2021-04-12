# Abstract
* a singleton class which manages all the pipe life  
* let the app developer add and use their own pipes with different types  
</br>

# API
* **pipe0\* add<T, P, F, S\>(pipeFunc/QJSValue aFunc, const QJsonObject& aParam = QJsonObject())**  
    - add a pipe into the pipeline  
    - return the pipe of the specific type  
    - if there is a pipe with the same name, the original pipe will be removed  
_sample_:
```
add<int, pipe, pipeFunc<int>, pipeFunc<int>>([](stream<int>* aInput){  //the second parameter is pipe as default, other choices can be such as pipePartial, pipeLocal, pipeDelegate, pipeBuffer; the third and the forth parameter must be the same, and should be only in pipeFunc<T> and QJSValue, the default is pipeFunc<T>
    aInput.out();
},  Json("name", "pipe0",  //the name of the pipe, if there is no name, it will be regarded as an anonymous pipe
         "thread", 2,  //which thread the pipe will be executed on. if it is 0, it will be executed on the current thread
         "replace", false, //the pipe will reserve the old pipe's next pipes if they are existed
         "before", "pipe1", //inject this pipe before the target pipe, it will be executed on the same thread of the target pipe
         "after", "pipe2", //work like "before"
         "around", "pipe3" //work like "before", replace the function of this pipe
         "befored", "pipe4",  //inject the target pipe before this pipe, the target pipe will be executed on the same thread of this pipe
         "aftered", "pipe5"  //work like "befored"
         "module", "module1"  //denote the pipe of which module, the default is "". only the `modules` in config of the `.rea` and "" module will be included into the pipeline
    ))

topo result: pipe0' = pipe4 -> pipe0 -> pipe5
             pipe... -> pipe0' -> pipe1 -> pipe...
             pipe... -> pipe2 -> pipe0' -> pipe...
             pipe... -> pipe3(replaced by pipe0') ->pipe...
```  
</br>

* **pipe0\* find(const QString& aName, bool needFuture = true)**  
    - find the specific pipe by name  
    - `needFuture` is used for the case that the pipe is not existed now, but will be existed in the future  
</br>

* **void run<T>(const QString& aName, T aInput, const QString& aTag = "", bool aTransaction = true, std::shared_ptr<QHash<QString, std::shared_ptr<stream0\>\>\> aScopeCache = nullptr)**  
    - execute the specific pipe by name and start a pipeline procedure  
    - `aTransaction` denotes whether to create atransaction for this whole procedure  
    - `aScopeCache` denotes the scopecache for the stream  
_sample_:  
```
run<int>("pipe0", 0, "service0")
```  
</br>

* **void runC<T>(const QString& aName, T aInput, const QString& aStreamID, const QString& aTag = "")**  
    - execute the specific pipe by name with the cached stream  
    - it is used with `cache` of stream for pipeDelegate  
</br>

* **void syncCall<T, F\>(const QString& aName, T aInput)**  
    - only execute the specific pipe synchronously on current thread  
</br>

* **std::shared_ptr<stream<T\>\> call(const QString& aName, T aInput = T())**  
    - only execute the specific pipe asynchronously  
    - return the result stream  
</br>

* **std::shared_ptr<stream<T\>\> input(T aInput = T(), const QString& aTag = "", bool aTransaction = true, std::shared_ptr<QHash<QString, std::shared_ptr<stream0\>\>\> aScopeCache = nullptr)**  
    - return the stream of the input  
    - `aTransaction` denotes whether to create atransaction for this whole procedure  
    - `aScopeCache` denotes the scopecache for the stream  
</br>

* **void remove(const QString& aName)**  
    - remove the specific pipe by name  
</br>

* **void removeAspect(const QString& aPipe, pipe0::AspectType aType, const QString& aAspect = "")**  
    - remove the aspect of the pipe  
    - `aType` is the aspect type  
    - if `aAspect` is nothing, all the aspects of this type will be removed  
</br>

# Test and Demo
test_rea.cpp: test1(), test2(), test3(), test11(), test12()  
</br>

