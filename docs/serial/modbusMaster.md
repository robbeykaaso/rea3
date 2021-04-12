# Abstract
a serial modbus master, the constructor config is QModbusDevice::SerialPortNameParameter, QModbusDevice::SerialParityParameter, QModbusDevice::SerialBaudRateParameter, QModbusDevice::SerialDataBitsParameter and QModbusDevice::SerialStopBitsParameter

# API Pipe
* **callSlave**  
    - send "func", "payload", and "server" to the slave  
    - input: QJsonObject  
    - output: QByteArray  
    - type: pipePartial  
</br>

* **modbusBoardcast**  
    - boardcast some information from serial  
    - output: QJsonObject  
    - type: pipe  
</br>

# Test and Demo
test_modbus.cpp  
</br>