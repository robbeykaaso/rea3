# Abstract
* the wrapper of pagelist0  

# Attribute
* name: QString: the instance name  
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
    - type: pipe  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - title: QJsonArray: each column title  
        - selects: QJsonArray: selected items indexes  
        - fontclr: QString: entry font color  
        - data: QJsonArray: entry data  
            - QJsonObject: each entry  
                - entry: QJsonArray: each column data  
        - entrycount: int: entrycount for each page  
        - pageindex: int: current page index  
    - output: input  
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
    - type: pipePartial  
    - pipeline: qml; listener  
    - output: QJsonArray  
_sample_:  
```
    Pipeline.find("_listViewSelected").nextF(function(aInput){
        console.log(aInput)
    }, "manual")
```  
</br>