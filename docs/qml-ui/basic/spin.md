# Abstract
* a spin edit component for qml  

# Attribute
* caption: Text object  
* background: Rectangle object: background  
* spin: Spin object: edit  
* ratio: double: the ratio between caption and background  
* cstyle: QtObject: the style singleton  
_sample_:  
```
    Spin{
        width: 180
        caption.text: qsTr("attribute555") + ":"
        ratio: 0.4
    }
```  
</br>