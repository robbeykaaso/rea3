# Abstract
* the wrapper of list0  

# Attribute
* name: QString: the instance name  
_sample_:  
```
    List{
        name: "hello"
    }
```  
</br>

# API Pipe
* **name + _updateListView**  
    - renew the list gui by model data  
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonObject:
        - title: QJsonArray: each column title  
        - selects: QJsonArray: selected items indexes  
        - fontclr: QString: entry font color  
        - data: QJsonArray: entry data  
            - QJsonObject: each entry  
                - entry: QJsonArray: each column data  
        - index: QJsonArray: whether to modify specific data or replace whole old data  
        - append: bool: whether to clear old entries  
    - output: input  
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
    - type: pipePartial  
    - pipeline: qml; listener  
    - output: QJsonArray: selected indexes  
_sample_:  
```
    Pipeline.find("_listViewSelected").nextF(function(aInput){
        console.log(aInput)
    }, "manual")
```  
</br>
