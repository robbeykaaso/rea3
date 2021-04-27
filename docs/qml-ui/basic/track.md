# Abstract
* the track component for qml  

# Attribute
* caption: QString: the Text object  
* slider: the Slider object  
* ratio: double: the ratio between caption and background  
* interval: int: the interval count  
* indexChanged: signal: it will be emitted when the value changes  
_sample_:  
```
    Track{
        width: 180
        caption.text: qsTr("attri4") + ":"
        ratio: 0.4
    }
```  
</br>