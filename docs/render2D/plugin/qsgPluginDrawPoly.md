# Abstract
* a draw polyline plugin for qsgBoard  

# API Pipe  
* **cancelDrawPoly_ + name**  
    - cancel current polyline drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

* **cancelDrawPolyLast_ + name**  
    - cancel the last point drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

* **completeDrawPoly_ + name**  
    - complete current polyline drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject: empty  
    - output: QJsonObject: 
        - shape_id: QJsonObject: empty  
</br>

# Performace  
* first click mouse left button to start drawing the polyline  
* click mouse right button to trig `updateQSGMenu` in order to  get menu item and commands include `cancelDrawPoly`, `cancelDrawPolyLast` and `completeDrawPoly`  
* trig `completeDrawPoly` to complete drawing the polyline and trig `addCommand`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "drawpoly")})
```