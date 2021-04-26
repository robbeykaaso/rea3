# Abstract
the wrapper of list0  

# Attribute
* name: the instance name  
_sample_:  
```
    PageList{
        name: "hello"
    }
```  
</br>

# API Pipe
* **name + _updateListView**  
    - renew the list gui by model data  
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
_sample_:  
```
    Pipeline.run("_updateListView", {title: ["cat", "dog", "sheep", "rat"],  //the list title names
                                        selects: [1, 3, 5],  //the selected items indexes
                                        entrycount: 3,
                                        pageindex: 1,
                                        data: [  //the datas, please ensure the columns length equaled to the titles length
                                        {entry: [4, 6, 2, 3]},  
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]}
                                        ]})
```  
</br>

* **name + _listViewSelected**  
    - output the selected indexes in the list  
    - output: QJsonArray  
    - type: pipePartial: tag: "manual"  
_sample_:  
```
    Pipeline.find("_listViewSelected").next(function(aInput){
        console.log(aInput)
    }, "manual", {vtype: "array"})  //"manual"
```  
</br>

# Test and Demo
test.qml: qsTr("pagelist")  
</br>