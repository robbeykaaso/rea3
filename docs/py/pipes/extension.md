# Abstract
* rea-python provides some common kinds of pipes, however there also be many common logics coupled with app's own business logic such as buffer, throttle and so on. rea-python permit the developer to customize his own pipe type  
_sample_:  
```
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

```

# Reference
[pipe](../pipe.md)