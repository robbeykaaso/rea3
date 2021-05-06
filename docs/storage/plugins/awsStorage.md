# Abstract
* an AWS storage module  

# Usage
* initialize the storage instance  
_sample_:  
```
    static awsStorage aws_storage("testminio", rea::Json("local", true)); //parameter: {local: true, ip: "http://192.168.1.122:9000", access: "xxxx", secret: "xxxx"} //local is to use minIO service on the current machine  
    aws_storage.initialize();
```
</br>

* use api pipes

# API Pipe
* **fileRoot + readJsonObject**  
    - read json  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QJsonObject: file data  
    - output: bool: whether is successful  
</br>

* **fileRoot + writeJsonObject**  
    - read json  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QJsonObject: file data  
    - output: bool: whether is successful  
</br>

* **fileRoot + readByteArray**  
    - read byte array  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QByteArray: file data  
    - output: bool: whether is successful  
</br>

* **fileRoot + writeByteArray**  
    - write byte array  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: QByteArray: file data  
    - output: bool: whether is successful  
* Notice  
    - <font color="red">directory path ends with `/` will be successfully make directory and get the false result</font><br />  
</br>

* **fileRoot + readCVMat**  
    - read image  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: cv::Mat: image data  
    - output: bool: whether is successful  
</br>

* **fileRoot + writeCVMat**  
    - write image  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: bool  
    - scope:  
        - path: QString: file path  
        - data: cv::Mat: image data  
    - output: bool: whether is successful  
</br>

* **fileRoot + deletePath**  
    - delete path  
    - type: pipeParallel  
    - pipeline: c++; trigger  
    - input: QString  
    - output: input  
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
