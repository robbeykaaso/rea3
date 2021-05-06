# Abstract
* a file system storage module  

# Usage
* initialize the storage instance  
_sample_:  
```
    static fsStorage local_storage("root0");   //"root0" is the fileRoot
    local_storage.initialize();  //initialize the api pipes
```
</br>

* use api pipes  

# API Pipe
* **fileRoot + readJsonObject**  
    - read json  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QJsonObject: file data  
    - output: bool: whether is successful  
    - thread: 10  
_sample_:  
```  
in(false, "testStorage", std::make_shared<scopeCache>()
                            ->cache<QString>("path", "testFS.json"))
->asyncCall("readJsonObject")
->scope()
->data<QJsonObject>("data")
```  
</br>

* **fileRoot + writeJsonObject**  
    - read json  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QJsonObject: file data  
    - output: bool: whether is successful  
    - thread: 11  
</br>

* **fileRoot + readByteArray**  
    - read byte array  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QByteArray: file data  
    - output: bool: whether is successful  
    - thread: 10  
</br>

* **fileRoot + writeByteArray**  
    - write byte array  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QByteArray: file data  
    - output: bool: whether is successful  
    - thread: 11  
* Notice  
    - <font color="red">directory path ends with `/` will be successfully make directory and get the false result</font><br />  
</br>

* **fileRoot + readImage**  
    - read image  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QImage: image data  
    - output: bool: whether is successful  
    - thread: 10  
</br>

* **fileRoot + writeImage**  
    - write image  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QImage: image data  
    - output: bool: whether is successful  
    - thread: 11  
</br>

* **fileRoot + deletePath**  
    - delete path  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: QString  
    - output: input  
    - thread: 11  
</br>

* **fileRoot + listFiles**  
    - list file paths of current directory  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: QString  
    - scope:  
        - data: std::vector<QString\>: result paths  
    - output: input  
</br>

* **fileRoot + listAllFiles**  
    - list file paths of current directory recursively  
    - type: pipePartial  
    - pipeline: c++; trigger  
    - input: QString  
    - scope:  
        - data: std::vector<QString\>: result paths  
    - output: input  
</br>

# Extension  
* interface  
_sample_:  
```  
    //extend the read/write image interface for js
    extendTrigger(bool, readImage, js);
    rea::pipeline::instance()->add<bool>([](rea::stream<bool>* aInput){
        if (aInput->data()){
            auto img = aInput->scope()->data<QImage>("data");
            auto uri = "data:image/png;base64, " + rea::QImage2Base64(img);
            aInput->scope()->cache<QString>("uri", uri);
        }
        aInput->out();
    }, rea::Json("after", "js_readImage"));
    rea::pipeline::instance()->add<bool>([](rea::stream<bool>* aInput){
        auto uri = aInput->scope()->data<QString>("uri");
        aInput->scope()->cache<QImage>("data", Uri2QImage(uri));
        aInput->out();
    }, rea::Json("name", "js_writeImage",
                 "aftered", "writeImage",
                 "external",  "js"));
```

* new storage type  
_sample_:  
```  
class fsStorage2 : public rea::fsStorage{
public:
    fsStorage2(const QString& aRoot = "") : fsStorage(aRoot){}
    void initialize() override{
        rea::pipeline::instance()->add<bool, rea::pipeParallel>([this](rea::stream<bool>* aInput) {
            QJsonObject dt;
            auto ret = readJsonObject(aInput->scope()->data<QString>("path"), dt);
            aInput->scope()->cache("data", dt);
            aInput->setData(ret)->out();
        }, rea::Json("name", m_root + "readJsonObject2", "thread", 12));
    }
};
```