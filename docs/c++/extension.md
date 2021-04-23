# Abstract  
* each pipe can only works correctly for its next pipes in his own pipeline. if other pipelines want to use these pipes correctly, the developer should manually extend the pipe interface  

# Macro
* **extendTrigger(aType, aPipe, aPipeline)**  
    - extend the pipe as a trigger  
    - `aType` is the stream data type  
    - `aPipe` is the pipe name  
    - `aPipeline` is the outside pipeline name  
    - generate the new interface `##aPipeline_##aPipe` for the outside pipeline  
</br>

* **extendListener(aType, aPipe, aPipeline)**  
    - extend the pipe as a listener  
    - `aType` is the stream data type  
    - `aPipe` is the pipe name  
    - `aPipeline` is the outside pipeline name  
    - generate the new interface `##aPipeline_##aPipe` for the outside pipeline  
</br>
