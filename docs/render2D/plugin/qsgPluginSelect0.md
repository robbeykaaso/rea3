# Abstract
* a qsgObject select plugin for qsgBoard and is a decorator of `qsgPluginTransform`  

# API Pipe
* **selectedChanged_ + name**  
    - get selected object ids  
    - type: pipe  
    - pipeline: c++; listener  
    - output: QSet<QString\> : selected object ids  
</br>

* **qsgObjectCanBeSelected_ + name**  
    - return whether the qsgObject can be selected. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: bool  
    - scope: 
        - target: qsgObject*: the selected object  
    - output: bool; result  
</br>

* **clearSelects_ + name**  
    - clear selected qsgObjects. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: QSet<QString\> : selected object ids  
</br>

* **selectAll_ + name**  
    - select all qsgObjects. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: QSet<QString\> : selected object ids  
</br>

# Performace  
* click mouse left button to select object and multi-select objects by `Ctrl`  
* `selectedChanged` will be trigged after the selected objects count changes  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "select0")})
```