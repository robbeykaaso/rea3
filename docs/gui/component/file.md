# Abstract
the file dialog component for qml  

# Attribute
* name: the instance name  
</br>

# API Pipe
* **name + _selectFile**  
    - open file dialog and select files or folder  
    - input: QJsonObject  
    - type: pipeDelegate: name + _fileSelected  
_sample_:  
```
Pipeline.run("_selectFile", {folder: true,  //select file or folder, the default is false
                              title: "hello",  //the dialog title, the default is "Please choose folder" and "Please choose files"
                              filter: ["Image files (*.jpg *.png *.jpeg *.bmp)"]  //the file filters, the default is ""
                             }, "manual2")  //the service tag
```  
</br>

* **name + _fileSelected**  
    - output the selected files or directory  
    - output: QJsonArray  
    - type: pipePartial  
</br>

# Test and Demo
test.qml: qsTr("file")  
</br>