# Abstract  
* an edit node plugin for qsgBoard  

# Performance  
* click mouse left button to select `poly` shape or nothing and trig `addCommand`  
* drag the node on the edge to move or insert node  
* click mouse middle button to delete the node  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "editnode")})
```