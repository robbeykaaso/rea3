from re import T
from .rea import pipeline, pipelines, stream, scopeCache

def parseJsons(aContent: str) -> list:
    ret = list()
    tt = aContent.split("}{")
    len_tt = len(tt)
    for i in range(len_tt):
        str = tt[i]
        if i > 0:
            str = "{" + str
        if i < len_tt - 1:
            str = str + "}"
        ret.append(str)
    return ret

def connectRemote(aLocal: str, aRemote: str, aWriteRemote, aClient: bool, aRemoteLocal: str):
    pipelines(aRemote)
    pipelines(aLocal).updateOutsideRanges({aRemote})
    pipelines().find("receiveFrom" + ("Server" if aClient else "Client")).next(aRemote + "_receiveRemote", (aLocal if aRemoteLocal == "" else aRemoteLocal) + ";any")
    pipelines().add(aWriteRemote, {"name": aRemote + "_sendRemote"})

class pipelineRemote(pipeline):
    def __init__(self, aRemoteName: str, aLocalName: str):
        super().__init__(aRemoteName)
        self.__m_localName = aLocalName
        
        def receiveRemote(aInput: stream):
            dt = aInput.data()
            cmd = dt.get("cmd", "")
            if cmd == "execute":
                scp = aInput.scope()
                scp_arr = dt.get("scope", [])
                len_scp = len(scp_arr)
                for i in range(0, len_scp - 1, 2):
                    scp.cache(scp_arr[i], scp_arr[i + 1])
                self.executeFromRemote(dt.get("name", ""), dt.get("data", None), dt.get("tag", ""), scp, dt.get("sync", dict({})), dt.get("needFuture", False), dt.get("from", ""))
            elif cmd == "remove":
                self.removeFromRemote(dt.get("name", ""))
            
        pipelines().add(receiveRemote, {"name": self.name() + "_receiveRemote"})
    
    def executeFromRemote(self, aName: str, aData, aTag: str, aScope: scopeCache, aSync: dict, aNeedFuture: bool, aFlag: str):
        pipelines(self.__m_localName).execute(aName, stream(aData, aTag, aScope), aSync, aNeedFuture, aFlag)

    def removeFromRemote(self, aName: str):
        pipelines(self.__m_localName).remove(aName, False)
    
    def remove(self, aName: str, aOutside: bool = False):
        pipelines().run(self.name() + "_sendRemote", {"cmd": "remove", "name": aName, "remote": "any"})
    
    def execute(self, aName: str, aStream: stream, aSync: dict = {}, aFutureNeed: bool = False, aFrom: str = ""):
        if aStream.scope().data("remote") is not None:
            lst = aStream.scope().toList()
            scp = []
            len_lst = len(lst)
            for i in range(0, len_lst - 1, 2):
                tp = type(lst[i + 1])
                if tp is str or tp is bool or tp is float or tp is int or tp is dict or tp is list:
                    scp.append(lst[i])
                    scp.append(lst[i + 1])
            pipelines().run(self.name() + "_sendRemote", {"cmd": "execute", 
                                                          "data": aStream.data(),
                                                            "name": aName,
                                                            "remote": self.name(),
                                                            "tag": aStream.tag(),
                                                            "scope": scp,
                                                            "sync": aSync,
                                                            "needFuture": aFutureNeed,
                                                            "from": self.name() if aFrom == "" else aFrom},
                                                            aStream.tag(),
                                                            aStream.scope())
                                                          

