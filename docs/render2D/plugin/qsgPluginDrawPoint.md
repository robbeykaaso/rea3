# Abstract
* a draw point plugin for qsgBoard  

# API Pipe  
* **cancelDrawPoint_ + name**  
    - cancel the last point drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

# Performace  
* first click mouse left button to draw point  
* click mouse right button to trig `updateQSGMenu` in order to get menu item and commands include `cancelDrawPoint`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "drawpoint")})
```