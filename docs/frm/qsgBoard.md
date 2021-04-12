# Abstract
a qtquick component for show and modify shapes or images. a container for the business plugins  

# API Pipe
* **updateQSGModel_ + name**  
    - show a qsgModel from QJsonObject
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe    
</br>

* **updateQSGAttr_ + name**  
    - renew one attribute of one object or the whole model in the qsgModel  
    - input: QJsonObject  
    - type: pipeDelegate: QSGAttrUpdated_ + name  
</br>

* **updateQSGAttrs_ + name**  
    - renew a set of attributes of one object or the whole model in the qsgModel  
    - input: QJsonArray  
    - type: pipeDelegate: QSGAttrUpdated_ + name  
</br>

* **QSGAttrUpdated_ + name**  
    - output the updated attribute modifications  
    - output: QJsonArray  
    - type: pipe  
</br>

* **updateQSGCtrl_ + name**  
    - remove the old plugins and install the new plugins  
    - input: QJsonArray  
    - output: QJsonArray  
    - type: pipe    
_sample_:  
```
Pipeline2.run("updateQSGCtrl_testbrd", [{type: "drawfree"}])  //switch to drawfree mode
```  
</br>

# Sample
```
import QSGBoard 1.0
QSGBoard{
    name: "testbrd"
    plugins: [{type: "transform"}]  //plugin id
    anchors.fill: parent
    Component.onDestruction: {
        beforeDestroy()  //it is necessary for safely destroy qsgBoard
    }
}
```  
</br>

# Reference
[qsgModel](qsgModel.md)  
[qsgBoardPlugin](qsgBoardPlugin.md)