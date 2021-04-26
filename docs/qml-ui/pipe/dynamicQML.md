# Abstract
the qmls loader component for qml  

# Attribute
* name: the folder name. it will load all the qml files in the directory `cwd/plugin/name`  
* signal loaded: it will be emitted once the qml is loaded  
_sample_:  
```
    DynamicQML{
        name: "menu"  //load the qmls in the menu folder
        onLoaded: function(aItem){
            mainmenu.addMenu(aItem)  // add a menu to the mainmenu
        }
    }
```  
</br>
