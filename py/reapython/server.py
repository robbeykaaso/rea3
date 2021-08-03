from PyQt5.QtCore import QObject, pyqtSignal
from PyQt5.QtNetwork import QHostAddress, QTcpServer, QTcpSocket
from .rea import pipelines, scopeCache
from .reaRemote import parseJsons
from time import sleep
import json

class normalServer(QObject):
    def __init__(self, aConfig: dict = {}):
        super().__init__(None)
        self.__m_socket = QTcpServer()
        self.__m_clients = set()
        self.__m_pipeline = aConfig.get("pipeline", "py")

        self.__m_socket.newConnection.connect(self.newConnect)
        self.__m_socket.listen(QHostAddress.LocalHost, 8081)

        pipelines(self.__m_pipeline).add(lambda aInput:
            aInput.out()
        , {"name": "receiveFromClient", "type": "Partial"})

        pipelines(self.__m_pipeline).add(lambda aInput:
            aInput.out()
        , {"name": "clientStatusChanged"})
        #self.connected = pyqtSignal()
    
    def __del__(self):
        self.__m_clients.clear()

    def disConnected(self):
        client = QObject.sender(self)
        pipelines(self.__m_pipeline).run("clientStatusChanged", False, "", scopeCache({"socket": client}))
        if client in self.__m_clients:
            self.__m_clients.remove(client)
            client.readyRead.disconnect(self.readMessage)
            client.disconnected.disconnect(self.disConnected)

    def newConnect(self):
        client = self.__m_socket.nextPendingConnection()
        client.readyRead.connect(self.readMessage)
        client.disconnected.connect(self.disConnected)
        pipelines(self.__m_pipeline).run("clientStatusChanged", True, "", scopeCache({"socket": client}))
        self.__m_clients.add(client)

    def readMessage(self):
        client = QObject.sender(self)
        while client.bytesAvailable():
            qba = client.readAll()
            strs = parseJsons(str(qba.data(), "utf-8"))
            for i in strs:
                req = json.loads(i)
                pipelines(self.__m_pipeline).run("receiveFromClient", req, req["remote"], scopeCache({"socket": client}))

    def writeSocket(self, aSocket: QTcpSocket, aData: dict):
        if aSocket in self.__m_clients:
            aSocket.write(bytearray(json.dumps(aData).encode("utf-8")))
            aSocket.flush()
            while aSocket.bytesToWrite() > 0:
                sleep(0.005)