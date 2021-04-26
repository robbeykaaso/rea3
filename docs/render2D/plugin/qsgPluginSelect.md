# Abstract
* a rich qsgObject select plugin for qsgBoard providing some editing functions and inherits form `qsgPluginSelect0`  

# API Pipe
* **updateSelectedMask_ + name**  
    - update the select handles locations. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; listener  
    - scope:  
        - selects: QSet<QString\> : the selected object ids  
    - output: pointList : the boundbox of selected objects in WCS  
</br>

* **deleteShapes_ + name**  
    - delete the specific or selected objects. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonArray : the object ids to delete/empty is all the selected objects  
    - output: input  
</br>

* **copyShapes_ + name**  
    - copy the selected objects to the memory. the api only works under this mode    
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject  
    - output: QJsonObject  
</br>

* **pasteShapes_ + name**  
    - paste the specific or copied objects. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject :  
        - shapes: QJsonArray : the object ids to paste/empty is all the copied objects  
    - output: input  
</br>

* **moveShapes_ + name**  
    - move the specific objects. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject :  
        - shapes: QJsonArray : the object ids to move  
        - del: QJsonArray : the moving vector : [x, y]  
    - output: input  
</br>

# Performance  
* click mouse left button to select object and multi-select objects by `Ctrl`  
* `updateSelectedMask` will be trigged after `selectedChanged`  
* drag the objects or handles to move and scale the objects  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "select")})
```