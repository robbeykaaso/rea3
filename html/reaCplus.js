class pipelineCPlus extends pipeline{

    constructor(){
        super("c++")
        this.candidates_functions = []
    }

    initPipelineJS(){
        return new Promise((resolve, reject) => {
            if (typeof qt != "undefined"){
                new QWebChannel(qt.webChannelTransport, (channel) => {
                    this.CplusLinker = channel.objects.Pipeline;
                    if (!this.CplusLinker)
                        alert("no c++ linker")
                    this.CplusLinker.executeJSPipe.connect(function(aName, aData, aTag, aScope, aSync, aFromOutside){
                        const len = Object.keys(aScope).length
                        let sp = {}
                        for (let i = 0; i < len; i += 2)
                            sp[aScope[i]] = aScope[i + 1]
                        pipelines().execute(aName, new stream(aData, aTag, new scopeCache(sp)), aSync, aFromOutside)
                    })
                    this.CplusLinker.removeJSPipe.connect(function(aName){
                        pipelines().remove(aName)
                    })
                    this.inited = true

                    for (let i in this.candidates_functions)
                        this.candidates_functions[i]()
                    this.candidates_functions = []

                    resolve()
                })
            }else{
                this.inited = true
                alert("qt object not exists!")
                reject()
            }
        })
    }

    init(aFunc){
        if (!this.inited){
            if (!this.candidates_functions.length)
                this.initPipelineJS()
            this.candidates_functions.push(aFunc)
        }else{
            aFunc()
        }
    }

    execute(aName, aStream, aSync, aFromOutside = false, aFlag = "any"){
        this.init(e=>{
            if (this.CplusLinker)
                this.CplusLinker.tryExecuteOutsidePipe(aName, aStream.data(), aStream.tag(), aStream.scope().m_data, aSync, aFlag)
        })
    }

    remove(aName, aOutside = false){
        this.init(e=>{
            if (this.CplusLinker)
                this.CplusLinker.removePipeOutside(aName)
        })
    }
}

pipelines().add(function(aInput){
    aInput.setData(new pipelineCPlus())
}, {name: "createc++pipeline"})

pipelines("c++").init(function(){})
