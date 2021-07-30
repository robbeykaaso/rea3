from rea import pipelines, pipeline, stream, pipe
from PyQt5.QtCore import QEvent
from time import sleep
import threading

#test py different type instance
def test0():
    pipelines().add(lambda aInput:
        print(aInput.data()) 
    , {"name": "test"})
    pipelines().add(lambda aInput:
        print(aInput.data()) 
    , {"name": "test2"})
    pipelines().run("test", "hello")
    pipelines().run("test", False)

pipelines().add(lambda aInput:
    print(aInput.data())
, {"name": "testSuccessPy"})

pipelines().add(lambda aInput:
    print(aInput.data())
, {"name": "testFailPy"})

#test py anonymous next
def test1():
    def p1(aInput: stream):
        assert aInput.data() == 3
        aInput.out()
    def p2(aInput: stream):
        assert aInput.data() == 3
        aInput.outs("Pass: test1", "testSuccessPy")
    pipelines() \
    .add(p1, {"name": "test1"}) \
    .nextF(p2)

    pipelines().run("test1", 3)

#test py specific next
def test2():
    def p1(aInput: stream):
        assert aInput.data() == 4
        aInput.outs(5, "test2_")
    def p2(aInput: stream):
        assert aInput.data() == 5
        aInput.outs("Pass: test2", "testSuccessPy")
    pipelines() \
    .add(p1, {"name": "test2"}) \
    .nextF(p2, "", {"name": "test2_"})

    pipelines().run("test2", 4)

#test py pipe future
def test3():
    def p1(aInput: stream):
        assert(aInput.data() == 66)
        aInput.outs("test3", "test3_0")
    pipelines().add(p1, {"name": "test3"}) \
    .next("test3_0") \
    .next("testSuccessPy")

    pipelines().add(lambda aInput:
        aInput.out()
    , {"name": "test3_1"}) \
    .next("test3__") \
    .next("testSuccessPy")

    pipelines().find("test3_0") \
    .nextF(lambda aInput:
        aInput.out()
    , "", {"name": "test3__"}) \
    .next("testSuccessPy")

    def p2(aInput: stream):
        assert aInput.data() == "test3"
        aInput.outs("Pass: test3", "testSuccessPy") 
        aInput.outs("Pass: test3_", "test3__")
    pipelines().add(p2, {"name": "test3_0"})

    pipelines().run("test3", 66)
    pipelines().run("test3_1", "Pass: test3__")

#test py pipe partial
def test4():
    def p1(aInput: stream):
        assert aInput.data() == 66
        aInput.setData(77).out()

    def p2(aInput: stream):
        assert aInput.data() == 77
        aInput.outs("Pass: test4", "testSuccessPy")

    def p3(aInput: stream):
        assert aInput.data() == 77
        aInput.outs("Fail: test4", "testFailPy")

    pipelines().add(p1, {"name": "test4", "type": "Partial"}) \
    .nextFB(p2, "test4") \
    .nextF(p3, "test4_")

    pipelines().run("test4", 66, "test4")

#test py pipe delegate and pipe param
def test5():
    def p1(aInput: stream):
        assert aInput.data() == 66.0
        aInput.out()
    pipelines().add(p1, {"name": "test5_0",
        "delegate": "test5",
        "type": "Delegate"}) \
    .next("testSuccessPy")

    def p2(aInput: stream):
        assert aInput.data() == 56.0
        aInput.setData("Pass: test5").out()
    pipelines().add(p2, {"name": "test5"})

    pipelines().run("test5_0", 66.0)
    pipelines().run("test5", 56.0)

#test py asyncCall
def test6():
    def p1(aInput: stream):
        #print(threading.currentThread().getName())
        aInput.setData(aInput.data() + 1).out()

    def p2(aInput: stream):
        #print(threading.currentThread().getName())
        assert aInput.data() == 1
        aInput.outs("world")

    def p3(aInput: stream):
        #print(threading.currentThread().getName())
        assert aInput.data() == "world"
        aInput.setData("Pass: test6").out()

    pipelines().input(0, "test6") \
        .asyncCallF(p1, {"thread": 2}) \
        .asyncCallF(p2, {"thread": 3}) \
        .asyncCallF(p3) \
        .asyncCall("testSuccessPy")

#test py aop and keep topo
def test7():
    def p1(aInput: stream):
        dt = aInput.data()
        assert dt == 1.0
        aInput.setData(dt + 1).out()
    pipelines().add(p1, {"name": "test__7", "before": "test_7", "replace": True})
    
    def p2(aInput: stream):
        dt = aInput.data()
        assert dt == 2.0
        aInput.setData(dt + 1).out()
    pipelines().add(p2, {"name": "test_7", "before": "test7", "replace": True})

    def p3(aInput: stream):
        dt = aInput.data()
        assert dt == 3.0
        aInput.setData(dt + 1).out()
    pipelines().add(p3, {"name": "test7", "replace": True})

    def p4(aInput: stream):
        dt = aInput.data()
        assert dt == 4.0
        aInput.setData(dt + 1).out()
    pipelines().add(p4, {"name": "test7_", "after": "test7", "replace": True})

    def p5(aInput: stream):
        dt = aInput.data()
        assert dt == 5.0
        aInput.outs("Pass: test7", "testSuccessPy")
    pipelines().add(p5, {"name": "test7__", "after": "test7_", "replace": True})

    pipelines().run("test7", 1)

#test py parallel
def test8():
    lock = threading.Lock()
    def p1(aInput: stream):
        aInput.scope().cache("threads", set({})).cache("count", 0)
        for i in range(200):
            aInput.outs(i)

    def p2(aInput: stream):     
        sleep(0.005)
        lock.acquire()
        trds = aInput.scope().data("threads")
        trds.add(threading.currentThread().getName())
        aInput.scope().cache("threads", trds)
        cnt = aInput.scope().data("count") + 1
        aInput.scope().cache("count", cnt)
        lock.release()
        aInput.out()
    
    def p3(aInput: stream):
        lock.acquire()
        sleep(0.005)
        cnt = aInput.scope().data("count")
        if cnt == 200:
            aInput.scope().cache("count", cnt + 1)
            assert len(aInput.scope().data("threads")) == 8
            aInput.outs("Pass: test8", "testSuccessPy")
        lock.release()

    pipelines().add(p1, {"name": "test8"}) \
        .nextP(pipelines().add(p2, {"name": "test8_", "type": "Parallel"})) \
        .nextF(p3, "test8", {"name": "test8__"})

    pipelines().run("test8", 0, "test8")

#test py remove aspect
def test9():
    pipelines().add(None, {"name": "test9"})
    pipelines().add(None, {"name": "test_9", "before": "test9"})
    pipelines().add(None, {"name": "test_9_", "before": "test9"})
    pipelines().add(None, {"name": "test9_", "before": "test9"})
    pipelines().find("test9").removeAspect("before", "test_9_")
    pipelines().find("test9").removeAspect("before", "test_9")
    pipelines().find("test9").removeAspect("before", "test9_")

#init c++ pipeline and link with py pipeline    
pipelines().add(lambda aInput:
    aInput.setData(pipeline("c++")).out()
, {"name": "createc++pipeline"})
pipelines().updateOutsideRanges({"c++"})
pipelines("c++").updateOutsideRanges({"py"})
#test pipe mixture: py->py.future(c++)->py.future(c++)->py; scopeCache
def test10():
    def p1(aInput: stream):
        aInput.scope().cache("hello", "world")
        aInput.out()

    def p2(aInput: stream):
        assert aInput.data() == 6
        assert aInput.scope().data("hello") is None
        assert aInput.scope().data("hello2") == "world"
        aInput.outs("Pass: test10", "testSuccessPy")

    pipelines().add(p1, {"name": "test10"}) \
    .next("test10_") \
    .next("test10__") \
    .nextF(p2, "", {"name": "test_10"})

    def p3(aInput: stream):
        assert aInput.data() == 4.0
        assert aInput.scope().data("hello") == "world"
        aInput.setData(aInput.data() + 1).out()
    pipelines("c++").add(p3, {"name": "test10_", "external": "py"})
    def p4(aInput: stream):
        assert aInput.data() == 5.0
        aInput.scope(True).cache("hello2", "world")
        aInput.setData(aInput.data() + 1).out()
    pipelines("c++").add(p4, {"name": "test10__", "external": "py"})

    pipelines().run("test10", 4)

#test pipe mixture: py.future(c++)->py
def test11():
    def p1(aInput: stream):
        assert aInput.data() == "world"
        aInput.outs("Pass: test11", "testSuccessPy")

    pipelines().find("test11") \
    .nextF(p1, "", {"name": "test_11"})

    def p2(aInput: stream):
        assert aInput.data() == "hello"
        aInput.setData("world").out()
    pipelines("c++").add(p2, {"name": "test11", "external": "py"})

    pipelines().run("test11", "hello")

#test pipe mixture partial: py.future(c++)->py
def test12():
    def p1(aInput: stream):
        assert aInput.data() == 77.0
        aInput.outs("Pass: test12", "testSuccessPy")
    def p2(aInput: stream):
        assert aInput.data() == 77.0
        aInput.outs("Fail: test12", "testFailPy")

    pipelines().find("test12") \
    .nextFB(p1, "test12", {"name": "test12_"}) \
    .nextF(p2, "test12_", {"name": "test12__"})

    def p3(aInput: stream):
        assert aInput.data() == 66
        aInput.setData(77).out()
    pipelines("c++").add(p3, {"name": "test12",
                              "type": "Partial", 
                              "external": "py"})

    pipelines().run("test12", 66, "test12")

#test pipe mixture delegate: c++->c++.future(py)->py
def test13():
    def p1(aInput: stream):
        assert aInput.data() == 56.0
        aInput.setData("Pass: test13").out()
    pipelines().add(p1, {"name": "test13", "external": "c++"})

    def p2(aInput: stream):
        assert aInput.data() == 66.0
        aInput.out()
    pipelines("c++").add(p2, {"name": "test13_0", 
                              "delegate": "test13",
                              "type": "Delegate"}) \
    .next("testSuccessPy")

    pipelines().run("test13_0", 66)
    pipelines().run("test13", 56)

#test pipe mixture: py.asyncCall.c++
def test14():
    def p1(aInput: stream):
        assert aInput.data() == 24.0
        aInput.outs("Pass: test14")
    pipelines("c++").add(p1, {"name": "test14", "thread": 5, "external": "py"})

    pipelines().input(24, "test14").asyncCall("test14").asyncCall("testSuccessPy")

#test custom py pipe
def test15():
    class pipeCustomPy(pipe):
        def event(self, e: QEvent) -> bool:
            if e.type() == QEvent.User + 1:
                if e.getName() == self._m_name:
                    stm = e.getStream()
                    stm.scope().cache("flag", "test15")
                    self.doEvent(stm)
                    self._doNextEvent(self.m_next, stm)
            return True

    def p1(aInput: stream):
        sp = aInput.scope()
        aInput.setData(pipeCustomPy(sp.data("parent"), sp.data("name")))
    pipelines().add(p1, {"name": "createPyPipeCustomPy"})
    def p2(aInput: stream):
        assert aInput.scope().data("flag") == "test15"
        aInput.outs("Pass: test15", "testSuccessPy")
    pipelines().add(p2, {"name": "test15", "type": "CustomPy"})

    pipelines().run("test15", 0)