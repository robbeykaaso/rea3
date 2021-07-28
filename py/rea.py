import uuid
from PyQt5.QtCore import QCoreApplication, QObject, QEvent, QThread

class stream:
    def __init__(self, aInput: any, aTag: str = "", aScope = None):
        self.__m_tag = aTag
        self.__m_data = aInput
        self.__m_scope = aScope
        self.m_outs = None
    def data(self) -> any:
        return self.__m_data
    def out(self, aTag: str = "") -> 'stream':
        if self.m_outs is None:
            self.m_outs = []
        if aTag != "":
            self.__m_tag = aTag
        return self
    def outs(self, aOut: any, aNext: str = "", aTag: str = "") -> 'stream':
        if self.m_outs is None:
            self.m_outs = []
        ret = stream(aOut, self.__m_tag if aTag == "" else aTag, self.__m_scope)
        self.m_outs.append([aNext, ret])
        return ret

class pipe(QObject):
    def __init__(self, aParent: 'pipeline', aName: str = "", aThread: int = 0):
        super().__init__()
        if aName == "":
            self.__m_name = str(uuid.uuid4())
        else:
            self.__m_name = aName
        self.__m_parent = aParent
        self.__m_next = {}
        self.__m_func = None
        self.__m_external = ""
        self.__m_thread = QThread.currentThread()
    
    def __insertNext(self, aName: str, aTag: str):
        self.__m_next[aName] = aTag

    def actName(self) -> str:
        return self.__m_name

    def external(self) -> str:
        return self.__m_external

    def next(self, aName: str, aTag: str) -> 'pipe':
        tags = aTag.split(";")
        for i in tags:
            self.__insertNext(aName, i)
        nxt = self.__m_parent.find(aName)
        return nxt

    def nextF(self, aFunc, aTag: str = "", aParam: dict = {}) -> 'pipe':
        return self.next(self.__m_parent.add(aFunc, aParam).actName(), aTag)

    def initialize(self, aFunc, aParam: dict = {}):
        self.__m_func = aFunc
        self.__m_external = aParam.get("external", self.__m_parent.name())
        pass

    def inPool(self, aReplace: bool):
        old = self.__m_parent.find(self.__m_name, False)
        if (old is not None):
            pass
        self.__m_parent.insertPipe(self.__m_name, self)

    class __streamEvent(QEvent):
        def __init__(self, aName, aStream: stream):
            super().__init__(QEvent.User + 1)
            self.__m_name = aName
            self.__m_stream = aStream
        def getName(self) -> str:
            return self.__m_name
        def getStream(self) -> stream:
            return self.__m_stream

    def event(self, e: QEvent) -> bool:
        if e.type() == QEvent.User + 1:
            if e.getName() == self.__m_name:
                stm = e.getStream()
                self._doEvent(stm)
                self._doNextEvent(self.__m_next, stm)
        return True

    def __tryExecutePipe(self, aName: str, aStream: stream):
        pip = self.__m_parent.find(aName)
        if pip.external() != self.__m_parent.name():
            self.__m_parent.tryExecutePipeOutside(pip.actName(), aStream, {}, pip.external())
        else:
            pip.execute(aStream)

    def _doNextEvent(self, aNexts: dict, aStream: stream):
        outs = aStream.m_outs
        aStream.m_outs = None
        if outs is not None:
            if len(outs):
                for i in outs:
                    if i[0] == "":
                        for j in aNexts.keys():
                            self.__tryExecutePipe(j, i[1])
                    else:
                        self.__tryExecutePipe(i[0], i[1])
            else:
                for i in aNexts.keys():
                    self.__tryExecutePipe(i, aStream)
    def _doEvent(self, aStream: stream):
        self.__m_func(aStream)

    def execute(self, aStream):
        if (QThread.currentThread() == self.__m_thread):
            QCoreApplication.sendEvent(self, self.__streamEvent(self.__m_name, aStream))
        else:
            QCoreApplication.postEvent(self, self.__streamEvent(self.__m_name, aStream))

m_pipelines = dict({})

class pipeline(QObject):
    def __init__(self, aName: str = "py"):
        self.__m_name = aName
        self.__m_threads = dict({})
        self.__m_pipes = dict({})
        self.__m_outside_pipelines = set({})

    def find(self, aName: str, aNeedFuture: bool = True) -> pipe:
        ret = self.__m_pipes.get(aName, None)
        if ret is not None and aNeedFuture:
            pass
        return ret

    def add(self, aFunc, aParam: dict = {}) -> pipe:
        nm = aParam.get("name", "")
        tmp = pipe(self, nm, aParam.get("thread", 0))
        tmp.inPool(aParam.get("replace", False))
        tmp.initialize(aFunc, aParam)
        return tmp

    def name(self) -> str:
        return self.__m_name

    def _tryExecutePipeOutside(self, aName: str, aStream: stream, aSync: dict, aFlag: str):
        for i, j in m_pipelines:
            if i in self.__m_outside_pipelines:
                if j != self:
                    if aFlag == "any":
                        j.execute(aName, aStream, aSync, True, self.name())
                    elif aFlag == j.name():
                        j.execute(aName, aStream, aSync)

    def _externalNexGot(self, aPipe: pipe, aStream: stream, aFrom: str) -> bool:
        if aPipe.external() != self.name() and aPipe.external() != aFrom:
            self._tryExecutePipeOutside(aPipe, aStream, {}, aPipe.external())
            return False
        return True

    def execute(self, aName: str, aStream: stream, aSync: dict = {}, aFutureNeed: bool = False, aFrom: str = ""):
        pip = self.find(aName, not aFutureNeed)
        if pip is None:
            return
        if aFrom != "" and not self._externalNexGot(pip, aStream, aFrom):
            return
        if (aSync):
            pass
        pip.execute(aStream)

    def insertPipe(self, aName: str, aPipe: pipe):
        self.__m_pipes[aName] = aPipe

    def input(self, aInput: any, aTag: str = "", aScope = None, aAutoTag: bool = False) -> stream:
        return stream(aInput, aTag, aScope)

    def run(self, aName: str, aInput: any, aTag: str = "", aScope = None) -> stream:
        stm = self.input(aInput, aTag, aScope)
        self.execute(aName, stm, {}, False, self.name())
        return stm

    def __del__(self):
        for i in self.__m_threads.values:
            if i.isRunning():
                i.terminate()
                i.wait()

def pipelines(aName: str = "py") -> pipeline:
    if aName not in m_pipelines:
        if aName == "py":
            m_pipelines[aName] = pipeline()
    return m_pipelines[aName]