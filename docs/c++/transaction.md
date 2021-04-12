# Abstract
* the whole procedure for one running procedure of the pipeline  
</br>

# API
* **void fail()**  
    - tag fail status for this transaction if exists  
</br>

* **void log(const QString& aLog)**  
    - add a log to this transaction if exists  
</br>

* **QString print()**  
    - print the detail of this transaction  
</br>

* **QString getName()**  
    - return the workname and tag of the first pipe  
</br>

# API Pipe
* **transactionStart**  
    - a trig to get the created transaction once it is created
    - output: transaction*  
    - type: trig  
_sample_:  
```
    rea::pipeline::add<transaction*>([](stream<transaction*>* aInput){
        auto rt = aInput->data();
        //do something
    }, rea::Json("name", "transactionStart"));
```
</br>

* **transactionEnd**  
    - a trig to get the ended transaction once it is destroyed  
    - output: QJsonObject  
    - type: trig  
_sample_:
```
    rea::pipeline::add<QJsonObject>([](stream<QJsonObject>* aInput){
        auto dt = aInput->data();
        //do something
    }, rea::Json("name", "transactionEnd"));
```
</br>