# Abstract
the editable combobox component for qml  

# Attribute
* caption: the Text object  
* background: the Rectangle object for background  
* combo: the EditCombo object for selecting  
* ratio: the ratio between caption and background  
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

# Test and Demo
test.qml: qsTr("baseCtrl")  
</br>