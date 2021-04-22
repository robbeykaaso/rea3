# Abstract
* a transform plugin for qsgBoard. Zoom and move the world coordinate system  

# API Pipe
* **updateQSGPos_ + name**  
    - output the `x`, `y`, `ratio` and `transform` information when the mouse is moving or wcs is zooming  
    - type: pipe  
    - pipeline: c++  
    - output: QJsonObject  
</br>

* **updateQSGMenu_ + name**  
    - output the custom menu items infomation when clicking mouse  
    - type: pipe  
    - pipeline: c++  
    - output: QJsonObject  
</br>