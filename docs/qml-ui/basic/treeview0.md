# Abstract  
* the treeview component for qml  

# Attribute
* imagePath: QJsonObject: the icon list, the key is decided by the `type` of item  
    - [type name]: QString: image path  
* selectWay: QString: the style of the selected item. it includes `color`, `background` and `bold`  
* selectColor: QString: the background color of selected item  
* openWay: QString: the way of opening folder. it includes `single`, `double` and `all`  
* setCurrentSelect: signal: it will be emitted when selecting one item, the output is `id` of item  

# API
* **buildDefaultTree(data, select)**  
    - generate the gui by data  
    - select is the `id` of the selected item  
_sample_:  
```
var tmp = [{
                "type": "folder",  //type
                "name": "folder0", 
                "id": "folder0",  //id
                "children": [{
                        "type": "page",
                        "name": "page0",
                        "id": "page0",
                        "children": [{
                                "id": "id0",
                                "type": "image",
                                "name": "image0",
                                "position": [],
                                "comment": "",
                                "source": "" //url
                            }, {
                                "id": "id1",
                                "type": "text",
                                "name": "text0",
                                "position": [],
                                "comment": "",
                                "relative_position": [],
                                "content": "",
                                "size": 16,
                                "color": "green",
                                "bold": ""
                            }, {
                                "id": "id2",
                                "type": "shape",
                                "name": "text0",
                                "position": [],
                                "comment": "",
                                "relative_position": [],
                                "direction": {
                                    "color": "green",
                                    "border": {
                                        "type": "line",
                                        "color": "red"
                                    },
                                    "radius": 30
                                }
                            }]
                    }, {
                        "type": "folder",
                        "name": "folder0",
                        "id": "folder1",
                        "children": [{
                                "id": "id3",
                                "type": "text",
                                "name": "text0_0",
                                "range": [0, 0, 50, 50],
                                "content": ""
                            }]
                    }]
            }]
buildDefaultTree(tmp, 'folder1')
```  
</br>