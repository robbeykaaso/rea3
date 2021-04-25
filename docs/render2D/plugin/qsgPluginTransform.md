# Abstract
* a transform plugin for qsgBoard. Zoom and move the world coordinate system  

# API Pipe
* **updateQSGPos_ + name**  
    - output the `x`, `y`, `ratio` and `transform` information when the mouse is moving or wcs is zooming  
    - type: pipe  
    - pipeline: c++; listener  
    - output: QJsonObject  
</br>

* **updateQSGMenu_ + name**  
    - output the custom menu items infomation when clicking mouse  
    - type: pipe  
    - pipeline: c++; listener  
    - output: QJsonObject  
</br>

# Performace  
* wheeling mouse will zoom-in and zoom-out the view  
* pressing mouse right button will trig `updateQSGMenu_`  
* any change of `x`, `y`, `ratio` about the WCS will trig `updateQSGPos_`  

# Install
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "transform")})
```