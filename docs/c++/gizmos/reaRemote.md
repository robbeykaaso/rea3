# Abstract
* a sidecar which is based on the qTcpsocket in order to provide the same syntax for developers to write the rea interface  

# Usage  
### low level  
* initialize `normalClient`,`normalServer` or your custom tcp component  
* switch the suitable pipelineRemote include `pipelineRemote` and `pipelineQMLRemote`  
* use `connectRemote` to connect the pipelineRemote and the tcp component  

### high level  
* fill the `.rea` config such as below:  
```
{
    "client": {  //the key could be client or server, if is client and "ip", "port", "id" all exists, the program will try to link server on the start  
        "ip": "127.0.0.1",
        "port": "8081",  //if is server, the type should be integer
        "id": "1234",
        "connects": [{
            "local": "c++",
            "remote": "c++_server",
            "remoteLocal": "c++"
        },
        {
            "local": "qml",
            "remote": "qml_server",
            "remotelocal": "qml",
            "qml": true  //whether to use pipelineQMLRemote
        }]
    },
    "process": ["process/process1/REA_SERVER.exe"]  //the child process you want to run together with the main process
}
```  