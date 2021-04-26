# Abstract
a gui generator by json for qml  

# API Pipe
* **rootkey + loadTreeView**  
generate the gui by `data` json and `style` json. its type is pipePartial  
_sample_:  
```
Pipeline2.run("loadTreeView", {data: sample}, {tag: "testTreeView"})
```  
</br>

* **rootkey + saveTreeView**  
output the json generated from gui. its type is pipePartial  
_sample_:  
```
Pipeline2.run("saveTreeView", {}, {tag: "testTreeView"})
```  
</br>

* **rootkey + modifyTreeViewGUI**  
dynamically modify the element of gui. its type is pipePartial  
_sample_:  
```
Pipeline2.run("modifyTreeViewGUI", {key: ["hi2", "1", "no", "1"], val: true}, {tag: "modifyTreeView"}) //modify the existed element
Pipeline2.run("modifyTreeViewGUI", {key: ["hi2", "1", "no", "2"], type: "add", val: 14}, {tag: "modifyTreeView"}) // add a new element
Pipeline2.run("modifyTreeViewGUI", {key: ["hi"], type: "del"}, {tag: "modifyTreeView"}) // delete an existed element
```  
</br>

* **rootkey + treeViewGUIModified**  
output the modified gui value and its location in the model  
_sample_:  
```
Pipeline2.find("treeViewGUIModified")
.next(function(aInput){
    console.log("treeViewGUIModified;" + aInput["key"] + ";" + aInput["val"] + ";" + aInput["type"])
})
```  
</br>

* **rootkey + styleTreeView**  
output a style json from a input json. the output "data" is the modified input, the output "style" is the style json. its type is pipePartial  
</br>

# Test and Demo
main.qml: treeview 
</br>
