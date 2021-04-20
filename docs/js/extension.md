# Abstract
* rea-js provides some common kinds of pipes and also permit the developer to customize his own pipe type  
_sample_:
```
    //0.define the pipe
    class pipeCustomJS extends pipe{
        execute(aStream){
            aStream.scope().cache("flag", "test49")
            this.doEvent(aStream)
            this.doNextEvent(this.m_next, aStream)
        }
    }
    //1.register the pipe type
    pipelines().add(function(aInput){
        const sp = aInput.scope()
        aInput.setData(new pipeCustomJS(sp.data("parent"), sp.data("name")))
    }, {name: "createJSPipeCustomJS"})
```

# Reference
[pipe](pipe.md)