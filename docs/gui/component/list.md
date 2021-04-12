# Abstract
the wrapper of list0  

# Attribute
* name: the instance name  
_sample_:  
```
    List{
        name: "hello"
    }
```  
* selectSuffix: the suffix for the tag of the manual trigged selected pipe  
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
                                        data: [  //the datas, please ensure the columns length equaled to the titles length
                                        {entry: [4, 6, 2, 3]},  
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]},
                                        {entry: [4, 6, 2, 3]}
                                        ]})

    Pipeline.run("_updateListView", {index: [2, 4, 5],  //if contains "index", the gui will modify the specific data rather than replace all old data
                                    fontclr: "red",  //the font color for each entry
                                    data: [
                                    {entry: [1, 3, 2, 3]},
                                    {},  //if there is no need to change old data, it could be empty
                                    {entry: [2, 3, 2, 3]}
                                    ]})
```  
</br>

* **name + _listViewSelected**  
    - output the selected indexes in the list  
    - output: QJsonArray  
    - type: pipePartial: tag: selectSuffix + "manual"  
_sample_:  
```
    Pipeline.find("_listViewSelected").next(function(aInput){
        console.log(aInput)
    }, "manual", {vtype: "array"})  //selectSuffix + "manual"
```  
</br>

# Test and Demo
test.qml: qsTr("list")  
</br>