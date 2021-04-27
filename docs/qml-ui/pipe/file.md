# Abstract
* the file dialog component for qml  

# Attribute
* name: QString: the instance name  
</br>

# API Pipe
* **name + _selectFile**  
    - open file dialog and select files or folder  
    - type: pipeDelegate : name + _fileSelected  
    - pipeline: qml; trigger  
    - input: QJsonObject:  
        - folder: select file or folder : bool  
        - title: the dialog title : QString  
        - filter: the file filters : QJsonArray  
        - save: save or read : bool  
_sample_:  
```
Pipeline.run("_selectFile", {folder: true,  //select file or folder, the default is false
                              title: "hello",  //the dialog title, the default is "Please choose folder" and "Please choose files"
                              filter: ["Image files (*.jpg *.png *.jpeg *.bmp)"],  //the file filters, the default is ""
                              save: true  //save dialog
                             }, "manual2")  //the service tag
```  
</br>

* **name + _fileSelected**  
    - output the selected files or directory  
    - type: pipePartial  
    - pipeline: qml; listener  
    - ouput: QJsonArray  
</br>