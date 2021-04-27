# Abstract
* the editable combobox component for qml  

# Attribute
* caption: Text object  
* background: Rectangle object: background  
* combo: EditCombo object  
* ratio: double: the ratio between caption and background  
_sample_:  
```
    ComboE{
        width: 180
        caption.text: Pipeline2.tr("attributeeee6") + ":"
        ratio: 0.4
        combo.modellist: ['test1', 'test2', 'test3', 'test4']
        combo.currentSelect: "test2"
    }
```  
</br>