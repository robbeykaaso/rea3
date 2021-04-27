# Abstract
* the uniform window component for qml  

# Attribute
* caption: QString: the title of the window  
* content: QMLObject: the content of the window  
* titlebuttons: QJsonArray: the head buttons on the title bar  
    - QJsonObject: each button  
        - cap: QString: button caption  
        - func: function: button function  
* footbuttons: QJsonArray: the foot buttons on the window foot  
    - QJsonObject: each button  
        - cap: QString: button caption  
        - func: function: button function  
_sample_:
```
    TWindow{
        id: twin
        caption: qsTr("TWindow")
        content: Column{
            anchors.fill: parent
            Rectangle{
                width: parent.width
                height: parent.height
                color: "gray"
            }
        }
        titlebuttons: [{cap: "O", func: function(){console.log("hello")}},  //cap is the text, func is the callback
                        {cap: "W", func: function(){console.log("world")}}]
        footbuttons: [{cap: "OK", func: function(){close()}},
                        {cap: "Cancel", func: function(){close()}}]
    }
```  
<center><p>![avatar](../../img/twindow.png)</p></center>  
</br>
