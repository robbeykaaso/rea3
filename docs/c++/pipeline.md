# Abstract
* manage self pipes' life  
* provide the interface to call pipe or run procedure  
</br>

# API
* **pipe0\* add<T, P, F, S\>(pipeFunc aFunc, const QJsonObject& aParam = QJsonObject())**  
    - add a pipe of the specific type into the pipeline  
    - `aFunc` is the function to be executed  
    - `aParam` is the pipe parameter  
    - `return` the pipe of the specific type  
_sample_:
```
add<int, pipe>([](stream<int>* aInput){  //the second parameter is pipe as default, other choices can be such as pipePartial, pipeDelegate, pipeAsync, pipeParallel
    aInput.out();
},  Json("name", "pipe0",  //the name of the pipe, if there is no name, it will be regarded as an anonymous pipe
         "thread", 2,  //which thread the pipe will be executed on. if it is 0, it will be executed on the current thread
         "replace", false, //the pipe will reserve the old pipe's next pipes if they are existed
         "before", "pipe1", //inject this pipe before the target pipe, it will be executed on the same thread of the target pipe
         "after", "pipe2", //work like "before"
         "around", "pipe3" //work like "before", replace the function of this pipe
         "befored", "pipe4",  //inject the target pipe before this pipe, the target pipe will be executed on the same thread of this pipe
         "aftered", "pipe5"  //work like "befored"
         "external", "js"  //decide which pipeline controls its next pipes
    ))

topo result: pipe0' = pipe4 -> pipe0 -> pipe5
             pipe... -> pipe0' -> pipe1 -> pipe...
             pipe... -> pipe2 -> pipe0' -> pipe...
             pipe... -> pipe3(replaced by pipe0') ->pipe...
```  
* Notice  
    - <font color="red">aop attributes include `before`, `after`, `around`, `befored`, and `aftered` could only work in the same pipeline</font><br />  
    - <font color="red">if there is a pipe with the same name, the original pipe will be removed</font><br />  
    - <font color="red">the `external` pipe's `next` could only be modified by the specific pipeline</font><br />  
    - <font color="red">thread 1 is occupied by `asyncCall`</font><br />  
</br>

* **pipe0\* find(const QString& aName, bool aNeedFuture = true)**  
    - find the specific pipe by name  
    - `aName` is the pipe name  
    - `aNeedFuture` is used for the case that the pipe is not existed now, but will be existed in the future or other pipelines  
    - `return` the pipe  
</br>

* **std::shared_ptr<stream<T\>\> run<T\>(const QString& aName, T aInput = T(), const QString& aTag = "", std::shared_ptr<scopeCache\> aScope = nullptr)**  
    - execute the specific pipe by name and start a business procedure  
    - `aName` is the pipe name  
    - `aInput` is the input stream data  
    - `aTag` is the tag name of the stream  
    - `aScope` is the stream additional data  
    - `return` the input stream  
_sample_:  
```
pipeline::instance()->run<int>("pipe0", 0, "service0")
```  
</br>

* **std::shared_ptr<stream<T\>\> call<T, F\>(const QString& aName, T aInput = T(), std::shared_ptr<scopeCache\> aScope = nullptr)**  
    - only execute the specific pipe and its aspects synchronously on current thread  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `aScope` is the stream additional data  
    - `return` the input stream  
</br>

* **std::shared_ptr<stream<T\>\> asyncCall(const QString& aName, T aInput = T())**  
    - only execute the specific pipe and its aspects asynchronously  
    - `aName` is the pipe name  
    - `aInput` is the input data  
    - `return` the output stream  
</br>

* **std::shared_ptr<stream<T\>\> input(T aInput = T(), const QString& aTag = "", std::shared_ptr<scopeCache\> aScopeCache = nullptr, bool aAutoTag = false)**  
    - generate a stream by the input data  
    - `aInput` is the input data  
    - `aTag` is the tag name of the stream  
    - `aScopeCache` is the stream additional data  
    - `aAutoTag` denotes whether to auto generating the tag if the input tag is ""  
    - `return` the generated stream  
_sample_:  
```
pipeline::instance()->input<int>(0)
in<int>(0) //this is an alternative gramma that can simplify the codes
```
</br>

* **void remove(const QString& aName, bool aOutside = false)**  
    - remove the specific pipe in this pipeline or in all pipelines  
    - `aName` is the pipe name  
    - `aOutside` is whether to remove the samed name pipe of all the pipelines  
</br>

* **QString name()**  
    - `return` the pipeline name  
</br>

* **void supportType<T\>(std::function<QVariant(stream0*)\> aTransfer)**  
    - permit the data of custom type(exclude the basic type of json) to be supported in qml or js stream  
    - `aTransfer` is how to transfer your class to QVariant  
_sample_:  
```
    //0.define your class
    class jsContext : public QObject{
        Q_OBJECT
    signals:
        void recvdMsg(const QString& msg);
    public slots:
        void onMsg(const QJsonValue& msg);
    public:
        Q_PROPERTY(QString name WRITE setName READ getName NOTIFY nameChanged)
        Q_INVOKABLE void test();
    }

    //1.support this type
    rea::pipeline::instance()->supportType<JsContext*>([](rea::stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<rea::stream<JsContext*>*>(aInput)->data());
    });
```
</br>

* **static pipeline* instance(const QString& aName = "")**  
    - get the pipeline instance by name  
    - `aName` is the pipeline name  
    - `return` the pipeline instance  
</br>