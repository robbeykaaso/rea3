# Abstract
a normal tcp socket server which maintains multi-clients

# API Pipe
* **callClient**  
    - send the json message to the clients with specific names, data type: clientMessage  
    - input: clientMessage  
    - output: clientMessage  
    - type: pipe  
</br>

* **receiveFromClient**  
    - got the message from a client  
    - output: clientMessage  
    - type: pipePartial  
    - tag: type in the message  
</br>

# Test and Demo
test_tcp.cpp    
</br>

# Reference
[normalClient](normalClient.md)