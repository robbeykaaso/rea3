# Abstract
* the api for QJson makes your codes more elligent 

# API
* **QJsonObject Json(...)**  
    - generate the QJsonObject  
_sample_:  
```
    Json("hello", "world");  // {hello: "world"}

    QJsonObject tmp;
    Json(tmp, "hello", "world");  //tmp = {hello: "world"}
```   
</br>

* **QJsonObject JArray(...)**  
    - generate the QJsonArray  
_sample_:  
```
    JArray("hello", "world");  // ["hello", "world"]

    QJsonObject tmp;
    JArray(tmp, "hello", "world");  //tmp = ["hello", "world"]
```
</br>



