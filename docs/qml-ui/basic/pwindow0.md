# Abstract
* the dynmaic content window inherited from `TWindow`  

# Attribute
* tr: function: the translate function  
* customGetParam: function: the get param function  
* accept: signal: be emitted on accepting  
* reject: signal: be emitted on rejecting  

# API  
* **showModel(aInput)**  
    - generate ui by input data  
    - `aInput` is the model data  
_sample_:  
```
    {
        title: "",
        content: {
            key1: {
                type: "spin", //default is when value type is number
                value: 0, //default is 0
                caption: "key1" //default is key
            },
            key2: {
                type: "check", //default is when value type is boolean
                value: false, //default is false
                caption: "key2" //default is key
            },
            key3: {
                type: "edit", //default is when value type is string
                value: "",  //default is ""
                caption: "key3",  //default is key
                trig: "" //the custom command to set edit value
            },
            key4: {
                type: "combo",
                value: 0, //default is 0
                caption: "key4",  //default is key
                model: ["sel1", "sel2"],  //the showed item list
                proto: ["pro1", "pro2"] //the actual item list, default is model
            },
            key5: {
                type: "comboe",
                value: "",  //default is "" or model[0]
                caption: "key5", //default is key
                model:["sel1", "sel2"]  //the item list
            }
        }
    }
```  
</br>

* **outputModel()**
    - return the editted key-value json object  
</br>