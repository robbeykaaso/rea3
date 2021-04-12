# Abstract
the supper and default class for storages, it supports file system as default. its operations are respectively on the thread 10(for reading) and 11(for writing)  

# API Pipe
* **afileRoot + readJson/writeJson**  
    - read/write the QJsonObject  
    - input: stgData<QJsonObject\>  
    - output: stgData<QJsonObject\>  
    - type: pipePartial  
_sample_:
```
    using stgJson = stgData<QJsonObject>;
    pipeline::run<stgJson>("" + "writeJson", stgJson(Json("hello", "world"), "testFS.js"), "service1");
```  
</br>

* **afileRoot + readJson2/writeJson2**  
    - read/write the QJsonObject  
    - input: QString(the path)  
    - output: QString  
    - varData: (the path: QJsonObject)  
    - type: pipePartial  
_sample_:
```
import Pipeline 1.0
Pipeline.run("writeJson2", "testFS2.json", "testQMLStg", false, {"testFS2.json": {hello: "world"}})
```  
</br>

* **afileRoot + readByteArray/writeByteArray**  
    - read/write the QByteArray  
    - input: stgData<QByteArray\>  
    - output: stgData<QByteArray\>  
    - type: pipePartial  
</br>

* **afileRoot + readDir/writeDir**  
    - read/write the std::vector<QByteArray\> to the directory  
    - input: stgData<std::vector<stgData<QByteArray\>\>\>  
    - output: stgData<std::vector<stgData<QByteArray\>\>\>  
    - type: pipePartial  
</br>

* **afileRoot + deletePath**  
    - delete the directory/file recursively  
    - input: QString  
    - output: QString  
    - type: pipePartial  
</br>

* **afileRoot + listFiles**  
    - list the files of the directory not recursively  
    - input: stgData<std::vector<QString\>\>  
    - output: stgData<std::vector<QString\>\>  
    - type: pipePartial  
</br>

* **afileRoot + listAllFiles**  
    - list the files of the directory recursively  
    - input: stgData<std::vector<QString\>\>  
    - output: stgData<std::vector<QString\>\>  
    - type: pipePartial  
</br>

* **afileRoot + listFiles2**  
    - list the files of the directory not recursively  
    - input: QString(the path)  
    - output: QString  
    - varData: (the path: QJsonArray) 
    - type: pipePartial  
</br>

* **afileRoot + listAllFiles2**  
    - list the files of the directory recursively  
    - input: QString(the path)  
    - output: QString  
    - varData: (the path: QJsonArray)  
    - type: pipePartial  
</br>

# Test and Demo
test_storage.cpp    

# Reference
[stgData](stgData.md)  
