# Abstract
* a draw free plugin for qsgBoard  

# API Pipe  
* **cancelDrawFree_ + name**  
    - cancel current free drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

* **cancelDrawFreeLast_ + name**  
    - cancel the last point drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

* **completeDrawFree_ + name**  
    - complete current free drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject: empty  
    - output: QJsonObject:  
        - shape_id: QJsonObject: empty  
</br>

* **drawFreeParam_ + name**  
    - set parameter for draw free. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonObject: empty uses default  
        - radius: the brush radius: int  
        - color: the brush color: QJsonArray(4)  
        - mode: QPainter::CompositionMode  
    - output: input: the current setting  
</br>

# Performace  
* first click mouse left button to start drawing free  
* click mouse right button to trig `updateQSGMenu` in order to  get menu item and commands include `cancelDrawFree`, `cancelDrawFreeLast` and `completeDrawFree`  
* trig `completeDrawFree` to complete drawing the polyline and trig `addCommand`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "drawfree")})
```