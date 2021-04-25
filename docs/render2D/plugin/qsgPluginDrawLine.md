# Abstract
* a draw line plugin for qsgBoard  

# API Pipe  
* **cancelDrawLine_ + name**  
    - cancel current drawing. the api only works under this mode  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: double  
    - output: input  
</br>

# Performace  
* first click mouse left button to start drawing the line  
* click mouse right button to trig `updateQSGMenu` in order to  get menu item and commands include `cancelDrawLine`  
* second click mouse left button to complete drawing the line and trig `addCommand`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "drawline")})
```