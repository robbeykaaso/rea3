# Abstract
a normal tcp socket client which link a server

# API Pipe
* **callServer**  
    - send the json message to the server  
    - input: QJsonObject  
    - type: pipeDelegate: receiveFromServer  
</br>

* **postServer**  
    - post the json message to the server  
    - input: QJsonObject  
    - type: pipePartial  
</br>

* **receiveFromServer**  
    - got the message from a server  
    - output: clientMessage  
    - type: pipePartial  
    - tag: type in the message  
</br>

* **tryLinkServer**  
    - either try link server which has specific "ip", "port", "id" or use ssdp protocal to link server  
    - input: QJsonObject  
    - output: QJsonObject  
    - type: pipe  
</br>

* **clientBoardcast**  
    - boardcast some information from socket  
    - output: QJsonObject  
    - type: pipe  
</br>

# Test and Demo
test_tcp.cpp  
</br>

# Reference
[normalServer](normalServer.md)