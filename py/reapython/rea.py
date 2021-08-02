import uuid
from time import sleep
from PyQt5.QtCore import QCoreApplication, QObject, QEvent, QRunnable, QThread, QEventLoop, QThreadPool

pipe_counter = 0
stream_counter = 0
class scopeCache:
    def __init__(self, aData: dict = {}):
        self.__m_data = aData

    def cache(self, aName: str, aData: any) -> 'scopeCache':
        self.__m_data[aName] = aData
        return self
    
    def data(self, aName: str) -> any:
        return self.__m_data.get(aName, None)

class stream:
    def __init__(self, aInput: any, aTag: str = "", aScope: scopeCache = None):
        self.__m_tag = aTag
        self.__m_data = aInput
        self.__m_scope = aScope
        self.m_outs = None
        global stream_counter
        stream_counter += 1

    def __del__(self):
        global stream_counter
        stream_counter -= 1

    def setData(self, aData: any) -> 'stream':
        self.__m_data = aData
        return self

    def scope(self, aNew: bool = False) -> scopeCache:
        if self.__m_scope is None or aNew:
            self.__m_scope = scopeCache({})
        return self.__m_scope

    def data(self) -> any:
        return self.__m_data

    def tag(self) -> str:
        return self.__m_tag

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

    def outsB(self, aOut: any, aNext: str = "", aTag: str = "") -> 'stream':
        self.outs(aOut, aNext, aTag)
        return self
    
    def noOut(self):
        self.m_outs = None

    def asyncCall(self, aName: str, aEventLevel: bool = True, aPipeline: str = "py") -> 'stream':
        ret = None
        line = pipelines(aPipeline)
        
        if aEventLevel:
            loop = QEventLoop()
            timeout = False
            def mn(aInput: stream):
                nonlocal ret, timeout
                ret = line.input(aInput.data(), aInput.tag(), aInput.scope())
                if loop.isRunning():
                    loop.quit()
                else:
                    timeout = True
            monitor = line.find(aName).nextF(mn, self.__m_tag)
            line.execute(aName, self)
            if not timeout:
                loop.exec()
            line.find(aName).removeNext(monitor.actName(), True, True)
        else:
            got_ret = False
            def mn(aInput: stream):
                nonlocal ret, got_ret
                ret = stream(aInput.data(), aInput.tag(), aInput.scope())
                got_ret = True
            monitor = line.find(aName).nextF(mn, self.__m_tag, {"thread": 1})
            line.execute(aName, self)
            while not got_ret:
                sleep(0.005)
            line.find(aName).removeNext(monitor.actName(), True, True)
        return ret

    def asyncCallF(self, aFunc, aParam: dict = {}, aEventLevel: bool = True, aPipeline: str = "py") -> 'stream':
        line = pipelines(aPipeline)
        pip = line.add(aFunc, aParam)
        ret = self.asyncCall(pip.actName(), aEventLevel, aPipeline)
        line.remove(pip.actName())
        return ret

class pipe(QObject):
    def __init__(self, aParent: 'pipeline', aName: str = "", aThreadNo: int = 0):
        super().__init__()
        if aName == "":
            self._m_name = str(uuid.uuid4())
        else:
            self._m_name = aName
        self._m_parent = aParent
        self.m_next = {}
        self.m_func = None
        self.m_external = aParent.name()
        self.m_before = ""
        self.m_after = ""
        self.m_around = ""
        if aThreadNo != 0:
            self.__m_thread = self._m_parent.findThread(aThreadNo)
            self.moveToThread(self.__m_thread)
        else:
            self.__m_thread = QThread.currentThread()
        global pipe_counter
        pipe_counter += 1

    def __del__(self):
        global pipe_counter
        pipe_counter -= 1

    def insertNext(self, aName: str, aTag: str):
        self.m_next[aName] = aTag

    def actName(self) -> str:
        return self._m_name

    def next(self, aName: str, aTag: str = "") -> 'pipe':
        tags = aTag.split(";")
        for i in tags:
            self.insertNext(aName, i)
        nxt = self._m_parent.find(aName)
        return nxt

    def nextP(self, aNext: 'pipe', aTag: str = "") -> 'pipe':
        tags = aTag.split(";")
        for i in tags:
            self.insertNext(aNext.actName(), i)
        return aNext

    def nextPB(self, aNext: 'pipe', aTag: str = "") -> 'pipe':
        self.nextP(aNext, aTag)
        return self

    def nextB(self, aName: str, aTag: str = "") -> 'pipe':
        self.next(aName, aTag)
        return self

    def nextF(self, aFunc, aTag: str = "", aParam: dict = {}) -> 'pipe':
        return self.next(self._m_parent.add(aFunc, aParam).actName(), aTag)

    def nextFB(self, aFunc, aTag: str = "", aParam: dict = {}) -> 'pipe':
        self.nextF(aFunc, aTag, aParam)
        return self

    def removeNext(self, aName: str, aAndDelete: bool = False, aOutside: bool = True):
        del self.m_next[aName]
        if aAndDelete:
            self._m_parent.remove(aName, aOutside)

    def setAspect(self, aTarget: str, aAspect: str) -> str:
        ret = aTarget
        if ret.find(aAspect) < 0:
            if ret != "":
                ret += ";"
            ret += aAspect
        return ret

    def removeAspect(self, aType: str, aAspect: str = None):
        def doRemoveAspect(aOriAspect: str):
            idx = aOriAspect.find(aAspect)
            len0 = len(aAspect)
            len1 = len(aOriAspect)
            if idx > 0:
                if idx + len0 == len1:
                    aOriAspect = aOriAspect[:idx - 1]
                else:
                    aOriAspect = aOriAspect[:idx - 1] + aOriAspect[idx + len0 - len1:] 
            elif idx == 0:
                if (len0 == len1):
                    aOriAspect = ""
                else:
                    aOriAspect = aOriAspect[idx + len0 + 1 - len1:] 
            return aOriAspect
        if aType == "before":
            if aAspect is None:
                self.m_before = ""
            else:
                self.m_before = doRemoveAspect(self.m_before)
        elif aType == "around":
            if aAspect is None:
                self.m_around = ""
            else:
                self.m_around = doRemoveAspect(self.m_around)
        elif aType == "after":
            if aAspect is None:
                self.m_after = ""
            else:
                self.m_after = doRemoveAspect(self.m_after)

    def doAspect(self, aName: str, aStream: stream) -> bool:
        if aName == "":
            return True
        ret = False
        nms = aName.split(";")
        for i in nms:
            pip = self._m_parent.find(i, False)
            if pip is not None:
                pip.doEvent(aStream)
                if aStream.m_outs is not None:
                    ret = True
        return ret

    def initialize(self, aFunc, aParam: dict = {}) -> 'pipe':
        self.m_func = aFunc
        self.m_external = aParam.get("external", self._m_parent.name())
        bf = aParam.get("befored", "")
        if bf != "":
            self.m_before = self.setAspect(self.m_before, bf)
        ed = aParam.get("aftered", "")
        if ed != "":
            self.m_after = self.setAspect(self.m_after, ed)
        return self

    def inPool(self, aReplace: bool):
        old = self._m_parent.find(self._m_name, False)
        if (old is not None and aReplace):
            self.replaceTopo(old)
        self._m_parent.insertPipe(self._m_name, self)

    def replaceTopo(self, aOldPipe: 'pipe'):
        self.m_next = aOldPipe.m_next
        self.m_before = aOldPipe.m_before
        self.m_around = aOldPipe.m_around
        self.m_after = aOldPipe.m_after

    def resetTopo(self):
        self.m_next = {}

    class _streamEvent(QEvent):
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
            if e.getName() == self._m_name:
                stm = e.getStream()
                self.doEvent(stm)
                self._doNextEvent(self.m_next, stm)
        return True

    def __tryExecutePipe(self, aName: str, aStream: stream):
        pip = self._m_parent.find(aName)
        if pip.m_external != self._m_parent.name():
            self._m_parent.tryExecutePipeOutside(pip.actName(), aStream, {}, pip.m_external)
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

    def doEvent(self, aStream: stream):
        if self.doAspect(self.m_before, aStream):
            if self.m_around != "":
                self.doAspect(self.m_around, aStream)
            else:
                self.call(aStream)
        if aStream.m_outs is not None:
            self.doAspect(self.m_after, aStream)

    def call(self, aStream: stream):
        if self.m_func is not None:
            self.m_func(aStream)

    def execute(self, aStream):
        if (QThread.currentThread() == self.__m_thread):
            QCoreApplication.sendEvent(self, self._streamEvent(self._m_name, aStream))
        else:
            QCoreApplication.postEvent(self, self._streamEvent(self._m_name, aStream))

class pipeFuture0(pipe):
    def __init__(self, aParent, aName: str = ""):
        super().__init__(aParent, aName)
        self.m_next2 = []
    
    def insertNext(self, aName: str, aTag: str):
        self.m_next2.append([aName, aTag])

class pipeFuture(pipe):
    def __init__(self, aParent, aName: str = ""):
        super().__init__(aParent)
        self.__m_act_name = aName
        self.__m_next2 = []
        if self._m_parent.find(aName + "_pipe_add", False) is not None:
            pip = pipeFuture0(self._m_parent, aName)
            pip.inPool(False)
            self._m_parent.call(aName + "_pipe_add")
            for i in pip.m_next2:
                self.insertNext(i[0], i[1])
            self.m_external = pip.m_external
            if pip.m_before != "":
                self.m_before = self.setAspect(self.m_before, pip.m_before)
            if pip.m_around != "":
                self.m_around = self.setAspect(self.m_around, pip.m_around)
            if pip.m_after != "":
                self.m_after = self.setAspect(self.m_after, pip.m_after)
            self._m_parent.remove(aName)
        
        def cb(aInput):
            pip = self._m_parent.find(aName, False)
            for i in self.__m_next2:
                pip.insertNext(i[0], i[1])
            pip.m_external = self.m_external
            if self.m_before != "":
                pip.m_before = pip.setAspect(pip.m_before, self.m_before)
            if self.m_around != "":
                pip.m_around = pip.setAspect(pip.m_around, self.m_around)
            if self.m_after != "":
                pip.m_after = pip.setAspect(pip.m_after, self.m_after)
            self._m_parent.remove(self._m_name)
            
        self._m_parent.add(cb, {"name": aName + "_pipe_add"})

    def resetTopo(self):
        self.__m_next2 = []

    def actName(self) -> str:
        return self.__m_act_name

    def insertNext(self, aName: str, aTag: str):
        self.__m_next2.append([aName, aTag])
    
    def removeNext(self, aName: str, aAndDelete: bool = False, aOutside: bool = True):
        self.__m_next2 = filter(lambda i: i[0] != aName, self.__m_next2)
        if aAndDelete:
            self._m_parent.remove(aName, aOutside)

    def execute(self, aStream):
        sync = {}
        if (len(self.__m_next2)):
            sync["next"] = self.__m_next2
        if (self.m_before != ""):
            sync["before"] = self.m_before
        if (self.m_around != ""):
            sync["around"] = self.m_around
        if (self.m_after != ""):
            sync["after"] = self.m_after
        self._m_parent.tryExecutePipeOutside(self.actName(), aStream, sync, "any")

m_pipelines = dict({})

class pipeline(QObject):
    def __init__(self, aName: str = "py"):
        super().__init__()
        self.__m_name = aName
        self.__m_threads = dict({})
        self.__m_pipes = dict({})
        self.__m_outside_pipelines = set({})
        if aName == "py":
            QThreadPool.globalInstance().setMaxThreadCount(8)

            def reportPyLeak(aInput: stream):
                print("py_pipe_counter: " + str(pipe_counter))
                print("py_stream_counter: " + str(stream_counter))
                aInput.out()
            self.add(reportPyLeak, {"name": "reportPyLeak"})

    def find(self, aName: str, aNeedFuture: bool = True) -> pipe:
        pip = self.__m_pipes.get(aName, None)
        if pip is None and aNeedFuture:
            pip = pipeFuture(self, aName)
            pip.inPool(False)
        return pip

    def add(self, aFunc, aParam: dict = {}) -> pipe:
        nm = aParam.get("name", "")
        tp = aParam.get("type", "")
        if tp == "":
            pip = pipe(self, nm, aParam.get("thread", 0))
        else:
            pip = pipelines().call("createPyPipe" + tp, "", scopeCache({"parent": self, "name": nm})).data()
        pip.inPool(aParam.get("replace", False))
        if nm != "":
            ad = pip.actName() + "_pipe_add"
            if ad in self.__m_pipes:
                self.call(ad, 0)
                self.remove(ad)
        pip.initialize(aFunc, aParam)

        bf = aParam.get("before", "")
        if bf != "":
            joint = self.find(bf)
            joint.m_before = joint.setAspect(joint.m_before, pip.actName())
        ar = aParam.get("around", "")
        if (ar != ""):
            joint = self.find(ar)
            joint.m_around = joint.setAspect(joint.m_around, pip.actName())
        af = aParam.get("after", "")
        if (af != ""):
            joint = self.find(af)
            joint.m_after = joint.setAspect(joint.m_after, pip.actName())
        return pip

    def _removePipeOutside(self, aName: str):
        for i, j in m_pipelines.items():
            if i in self.__m_outside_pipelines and j != self:
                j.remove(aName)

    def remove(self, aName: str, aOutside: bool = False):
        pip = self.__m_pipes.get(aName, None)
        if pip is None:
            pip = self.find(aName + "_pipe_add", False)
            if (pip is not None):
                pip = pipeFuture0(self, aName)
                pip.inPool(False)
                self.call(aName + "_pipe_add", 0)
                self.remove(aName + "_pipe_add")
                self.remove(aName)
        else:
            del self.__m_pipes[aName]
        if aOutside:
            self._removePipeOutside(aName)

    def call(self, aName: str, aInput: any = 0, aScope: scopeCache = None, aAOP: bool = True) -> stream:
        pip = self.__m_pipes.get(aName, None)
        stm = self.input(aInput, "", aScope)
        if pip is not None:
            if aAOP:
                pip.doEvent(stm)
            else:
                pip.call(stm)
        return stm

    def name(self) -> str:
        return self.__m_name

    def updateOutsideRanges(self, aRanges: set):
        for i in aRanges:
            self.__m_outside_pipelines.add(i)

    def tryExecutePipeOutside(self, aName: str, aStream: stream, aSync: dict, aFlag: str):
        for i, j in m_pipelines.items():
            if i in self.__m_outside_pipelines and j != self:
                if aFlag == "any":
                    j.execute(aName, aStream, aSync, True, self.name())
                elif aFlag == j.name():
                    j.execute(aName, aStream, aSync)

    def _externalNexGot(self, aPipe: pipe, aStream: stream, aFrom: str) -> bool:
        if aPipe.m_external != self.name() and aPipe.m_external != aFrom:
            self.tryExecutePipeOutside(aPipe.actName(), aStream, {}, aPipe.m_external)
            return False
        return True

    def execute(self, aName: str, aStream: stream, aSync: dict = {}, aFutureNeed: bool = False, aFrom: str = ""):
        pip = self.find(aName, not aFutureNeed)
        if pip is None:
            return
        if aFrom != "" and not self._externalNexGot(pip, aStream, aFrom):
            return
        pip = self.find(aName)
        if aSync:
            pip.resetTopo()
            nxt = aSync.get("next", [])
            if len(nxt):
                for i in nxt:
                    pip.insertNext(i[0], i[1])
            bf = aSync.get("before", "")
            if bf != "":
                pip.m_before = pip.setAspect(pip.m_before, bf)
            af = aSync.get("after", "")
            if af != "":
                pip.m_after = pip.setAspect(pip.m_after, af)
            ar = aSync.get("around", "")
            if ar != "":
                pip.m_around = pip.setAspect(pip.m_around, ar)
        pip.execute(aStream)

    def insertPipe(self, aName: str, aPipe: pipe):
        self.__m_pipes[aName] = aPipe
    
    def findThread(self, aNo: int) -> QThread:
        if not aNo in self.__m_threads:
            tmp = QThread()
            tmp.start()
            self.__m_threads[aNo] = tmp
            return tmp
        else:
            return self.__m_threads.get(aNo, None)

    def input(self, aInput: any, aTag: str = "", aScope: scopeCache = None, aAutoTag: bool = False) -> stream:
        tag = str(uuid.uuid4()) if aAutoTag and aTag == "" else aTag
        return stream(aInput, tag, aScope)

    def run(self, aName: str, aInput: any, aTag: str = "", aScope: scopeCache = None) -> stream:
        stm = self.input(aInput, aTag, aScope)
        self.execute(aName, stm, {}, False, self.name())
        return stm

    def __del__(self):
        for i in self.__m_threads.values:
            if i.isRunning():
                i.terminate()
                i.wait()

class pipeAsync(pipe):
    def __init__(self, aParent: 'pipeline', aName: str = "", aThreadNo: int = 0):
        super().__init__(aParent, aName, aThreadNo)

    def execute(self, aStream):
        QCoreApplication.postEvent(self, self._streamEvent(self._m_name, aStream))

class pipePartial(pipe):
    def __init__(self, aParent: 'pipeline', aName: str = "", aThreadNo: int = 0):
        super().__init__(aParent, aName, aThreadNo)
        self.m_next2 = {}
    
    def insertNext(self, aName: str, aTag: str):
        if aTag not in self.m_next2:
            self.m_next2[aTag] = {}
        self.m_next2[aTag][aName] = aTag

    def removeNext(self, aName: str, aAndDelete: bool = False, aOutside: bool = True):
        for i in self.m_next2:
            if aName in i:
                del i[aName]
        if aAndDelete:
            self._m_parent.remove(aName, aOutside)
    
    def replaceTopo(self, aOldPipe: 'pipe'):
        super().replaceTopo(aOldPipe)
        self.m_next2 = aOldPipe.m_next2
    
    def resetTopo(self):
        self.m_next2 = {}
    
    def event(self, e: QEvent) -> bool:
        if e.type() == QEvent.User + 1:
            if e.getName() == self._m_name:
                stm = e.getStream()
                self.doEvent(stm)
                self._doNextEvent(self.m_next2[stm.tag()], stm)
        return True

class pipeDelegate(pipe):
    def __init__(self, aParent: 'pipeline', aName: str, aThreadNo: int = 0):
        super().__init__(aParent, aName, aThreadNo)
        self.m_next2 = []

    def next(self, aNext: str, aTag: str = ""):
        self._m_parent.find(self.__m_delegate).next(aNext, aTag)
    
    def removeNext(self, aName: str, aAndDelete: bool = False, aOutside: bool = True):
        self._m_parent.find(self.__m_delegate).removeNext(aName, aAndDelete, aOutside)

    def insertNext(self, aName: str, aTag: str):
        self.m_next2.append([aName, aTag])
    
    def replaceTopo(self, aOldPipe: 'pipe'):
        super().replaceTopo(aOldPipe)
        self.m_next2 = aOldPipe.m_next2
    
    def resetTopo(self):
        self.m_next2 = []
        self._m_parent.find(self.__m_delegate).resetTopo()
    
    def initialize(self, aFunc, aParam: dict) -> 'pipe':
        self.__m_delegate = aParam.get("delegate", "")
        dl = self._m_parent.find(self.__m_delegate)
        for i in self.m_next2:
            dl.insertNext(i[0], i[1])
        return super().initialize(aFunc, aParam)

    def event(self, e: QEvent) -> bool:
        if e.type() == QEvent.User + 1:
            if e.getName() == self._m_name:
                stm = e.getStream()
                self.doEvent(stm)
        return True

class pipeParallel(pipePartial):

    class parallelTask(QRunnable):
        def __init__(self, aPipe: 'pipeParallel', aStream: stream):
            super().__init__()
            self.__m_pipe = aPipe
            self.__m_source = aStream

        def run(self):
            self.__m_pipe.doEvent(self.__m_source)
            self.__m_pipe._doNextEvent(self.__m_pipe.m_next2[self.__m_source.tag()], self.__m_source)

    def __init__(self, aParent: 'pipeline', aName: str = "", aThreadNo: int = 0):
        super().__init__(aParent, aName, aThreadNo)

    def initialize(self, aFunc, aParam: dict = {}) -> 'pipe':
        self.__m_act_name = aParam.get("delegate", "")
        self.__m_init = aFunc is not None
        return super().initialize(aFunc, aParam)
    
    def event(self, e: QEvent) -> bool:
        if e.type() == QEvent.User + 1:
            if e.getName() == self._m_name:
                if not self.__m_init:
                    pip = self._m_parent.find(self.__m_act_name, False)
                    if pip is not None:
                        self.m_func = pip.m_func
                    self.__m_init = True
                stm = e.getStream()
                QThreadPool.globalInstance().start(self.parallelTask(self, stm))
        return True

def pipelines(aName: str = "py") -> pipeline:
    if aName not in m_pipelines:
        if aName == "py":
            m_pipelines[aName] = pipeline()
        else:
            m_pipelines[aName] = pipelines().call("create" + aName + "pipeline").data()
    return m_pipelines[aName]

def createDelegatePipe(aInput: stream):
    sp = aInput.scope()
    aInput.setData(pipeDelegate(sp.data("parent"), sp.data("name")))
pipelines().add(createDelegatePipe, {"name": "createPyPipeDelegate"})

def createPartialPipe(aInput: stream):
    sp = aInput.scope()
    aInput.setData(pipePartial(sp.data("parent"), sp.data("name")))
pipelines().add(createPartialPipe, {"name": "createPyPipePartial"})

def createParallelPipe(aInput: stream):
    sp = aInput.scope()
    aInput.setData(pipeParallel(sp.data("parent"), sp.data("name")))
pipelines().add(createParallelPipe, {"name": "createPyPipeParallel"})

def createAsyncPipe(aInput: stream):
    sp = aInput.scope()
    aInput.setData(pipeAsync(sp.data("parent"), sp.data("name")))
pipelines().add(createAsyncPipe, {"name": "createPyPipeAsync"})