# Abstract
the track component for qml  

# Attribute
* caption: the Text object  
* slider: the Slider object  
* ratio: the ratio between caption and background  
* interval: the interval count  
* signal indexChanged: it will be emitted when the value changes  
_sample_:  
```
    Track{
        width: 180
        caption.text: qsTr("attri4") + ":"
        ratio: 0.4
    }
```  
</br>

# Test and Demo
test.qml: qsTr("baseCtrl")  
</br>