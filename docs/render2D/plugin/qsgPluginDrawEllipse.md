# Abstract
* a draw ellipse plugin for qsgBoard  

# API Pipe  
* **cancelDrawEllipse_ + name**  
    - cancel current drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

# Performace  
* first click mouse left button to start drawing the ellipse  
* click mouse right button to trig `updateQSGMenu` in order to get menu item and commands include `cancelDrawEllipse`  
* second click mouse left button to complete drawing the ellipse and trig `addCommand`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "drawellipse")})
```