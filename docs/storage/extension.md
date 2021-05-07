# Abstract
* developers can extend the storage module for its interface and functions  

# Sample
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

* new storage type and or functions  
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