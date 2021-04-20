# Abstract  
* an additional data container based on the stream  

# API  
* **std::shared_ptr<scopeCache\> cache<T\>(const QString& aName, T aData)**  
    - cache the data on self  
    - `aName` is the data name  
    - `aData` is the data  
    - `return` self  
</br>

* **T data(const QString& aName)**  
    - `aName` is the data name  
    - `return` the data by name  
</br>