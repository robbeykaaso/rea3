//#region utils

var scripts = [];
var modules = {};

function getModuleExport(aName, aObj){
    return modules[aName] && modules[aName][aObj];
}

function setModuleExport(aName, aObj, aTarget){
    if (!modules[aName])
        modules[aName] = {};
    if (modules[aName][aObj])
        alert('error: ' + aObj + ' is existed! Please rename');
    else
        modules[aName][aObj] = aTarget;
}

function extend(child, parent){
    var Super = function(){};
    Super.prototype = parent.prototype;
    child.prototype = new Super();
//    child.prototype.constructor = child;
}

//copy from https://blog.csdn.net/zhaiwenyuan/article/details/72781609
function getURLParam(url) { //获取url中"?"符后的字串
   var theRequest = new Object();
   if (url.indexOf("?") != -1) {
       var str = url.substr(1);
       strs = str.split("&");
       for (var i = 0; i < strs.length; i++) {
           theRequest[strs[i].split("=")[0]] = decodeURIComponent(strs[i].split("=")[1]);
       }
   }
   return theRequest;
}

function windowWidth(){
    return window.innerWidth
        || document.documentElement.clientWidth
        || document.body.clientWidth;
}

function windowHeight(){
    return window.innerHeight
        || document.documentElement.clientHeight
        || document.body.clientHeight;
}

function dynamicLoadCss(url, aCallback) {  //copy from: https://www.cnblogs.com/morang/p/dynamicloadjscssiframe.html
    //console.log(url);
    let args = Array.prototype.slice.apply(arguments);
    args.splice(0, 2);
    let head = document.getElementsByTagName('head')[0];
    let link = document.createElement('link');
    link.type='text/css';
    link.rel = 'stylesheet';
    link.onload = function(){
        if (aCallback)
            aCallback(...args);
    };

    link.href = url;
    head.appendChild(link);
    return link;
}

function installScript(path, aCallback){  //ref from: https://www.cnblogs.com/croso/p/5294251.html
    let args = Array.prototype.slice.apply(arguments);
    args.splice(0, 2);

    if (scripts.indexOf(path) >= 0){
        scripts.push(path);
        if (aCallback)
            aCallback(...args);
        return;
    }

    let head= document.getElementsByTagName('head')[0];
    let script= document.createElement('script');
    script.type= 'text/javascript';
    script.onload = function() {
// if (!this.readyState || this.readyState === "loaded" || this.readyState === "complete" ) {

    // Handle memory leak in IE
       // script.onload = script.onreadystatechange = null;
        if (aCallback)
            aCallback(...args);
    };

    script.src= path;
    head.appendChild(script);
    return script;
}

//https://blog.csdn.net/qq_39425958/article/details/87642137
function generateUUID() {
    var s = [];
    var hexDigits = "0123456789abcdef";
    for (var i = 0; i < 36; i++) {
      s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
    s[14] = "4"; // bits 12-15 of the time_hi_and_version field to 0010
    s[19] = hexDigits.substr((s[19] & 0x3) | 0x8, 1); // bits 6-7 of the clock_seq_hi_and_reserved to 01
    s[8] = s[13] = s[18] = s[23] = "-";
   
    var uuid = s.join("");
    return uuid
}

//#endregion

//#region rea-js

class scopeCache {
    constructor(aData = {}){
        this.m_data = aData
    }
    cache(aName, aData){
        this.m_data[aName] = aData
        return this
    }
    data(aName){
        return this.m_data[aName]
    }
}

class stream {
    constructor(aInput, aTag = "", aScope = null){
        this.m_data = aInput
        this.m_tag = aTag
        this.m_scope = aScope
    }
    setData(aData){
        this.m_data = aData
        return this
    }
    scope(aNew = false){
        if (!this.m_scope || aNew){
            this.m_scope = new scopeCache()
        }
        return this.m_scope
    }
    data(){
        return this.m_data
    }
    tag(){
        return this.m_tag
    }
    out(aTag = ""){
        if (!this.m_outs)
            this.m_outs = []
        if (aTag != "")
            this.m_tag = aTag
        return this
    }
    outs(aOut, aNext = "", aTag = ""){
        if (!this.m_outs)
            this.m_outs = []
        let ret = new stream(aOut, aTag == "" ? this.m_tag : aTag, this.m_scope)
        this.m_outs.push([aNext, ret])
        return ret
    }

    outsB(aOut, aNext = "", aTag = ""){
        outs(aOut, aNext, aTag)
        return this
    }

    noOut(){
        this.m_outs = null
    }

    async asyncCallS(){
        let stm = this
        for (const i in arguments)
            if (typeof arguments[i] == "string")
                stm = await stm.asyncCall(arguments[i])
            else
                stm = await stm.asyncCallF(arguments[i][0], arguments[i][1])
        return stm
    }

    async asyncCall(aName, aPipeline = "js"){
        let ret
        let got_ret = false

        let line = pipelines(aPipeline)
        const monitor = line.find(aName).nextF(aInput => {
            ret = new stream(aInput.data(), this.m_tag, aInput.scope())
            got_ret = true

        }, this.m_tag)
        line.execute(aName, this)

        function sleep(time) {
          return new Promise(resolve => setTimeout(resolve,time))
        }
        while(!got_ret)
            await sleep(5)
        line.find(aName).removeNext(monitor.actName(), true, false)

        return ret
    }
    async asyncCallF(aFunc, aParam = {}, aPipeline = "js"){
        let line = pipelines(aPipeline)
        const pip = line.add(aFunc, aParam)
        const ret = await this.asyncCall(pip.actName())
        line.remove(pip.actName())
        return ret
    }
}

class pipe {

    constructor(aParent, aName){
        this.m_parent = aParent
        this.m_external = aParent.name()
        this.m_next = {}
        if (aName == null || aName == "")
            this.m_name = generateUUID()
        else
            this.m_name = aName
    }
    
    inPool(aReplace){
        let old = this.m_parent.find(this.m_name, false)
        if (old && aReplace)
            this.replaceTopo(old)
        this.m_parent.m_pipes[this.m_name] = this
    }

    replaceTopo(aOldPipe){
        this.m_next = aOldPipe.m_next
        this.m_before = aOldPipe.m_before
        this.m_around = aOldPipe.m_around
        this.m_after = aOldPipe.m_after
    }

    resetTopo(){
        this.m_next = {}
    }

    actName() {
        return this.m_name
    }

    next(aName, aTag = ""){
        const tags = aTag.split(";")
        for (let i in tags)
            this.insertNext(aName, tags[i])
        return this.m_parent.find(aName)
    }

    nextB(aName, aTag = ""){
        this.next(aName, aTag)
        return this
    }

    nextF(aFunc, aTag = "", aParam = {}){
        return this.next(this.m_parent.add(aFunc, aParam).actName(), aTag)
    }

    nextFB(aFunc, aTag = "", aParam = {}){
        this.nextF(aFunc, aTag, aParam)
        return this
    }

    removeNext(aName, aAndDelete = false, aOutside = true){
        delete this.m_next[aName]
        if (aAndDelete)
            this.m_parent.remove(aName, aOutside)
    }

    initialize(aFunc, aParam){
        this.m_func = aFunc
        if (aParam && aParam["external"] != null)
            this.m_external = aParam["external"]
        let bf = aParam["befored"]
        if (bf)
            this.m_before = this.setAspect(this.m_before, bf)
        let ed = aParam["aftered"]
        if (ed)
            this.m_after = this.setAspect(this.m_after, ed)
        return this
    }

    execute(aStream){
        this.doEvent(aStream)
        this.doNextEvent(this.m_next, aStream)
    }

    insertNext(aName, aTag){
        this.m_next[aName] = aTag
    }

    doEvent(aStream){
        if (this.doAspect(this.m_before, aStream)){
            if (this.m_around)
                this.doAspect(this.m_around, aStream)
            else
                this.m_func(aStream)
        }
        if (aStream.m_outs)
            this.doAspect(this.m_after, aStream)
    }

    tryExecutePipe(aName, aStream){
        const pip = this.m_parent.find(aName)
        if (pip)
            if (pip.m_external != this.m_parent.name())
                this.m_parent.tryExecutePipeOutside(aName, aStream, {}, pip.m_external)
            else
                pip.execute(aStream)
    }

    doNextEvent(aNexts, aStream){
        const outs = aStream.m_outs
        aStream.m_outs = null
        if (outs)
            if (outs.length){
                for (let i in outs){
                    if (outs[i][0] == "")
                        for (let j in aNexts)
                            this.tryExecutePipe(j, outs[i][1])
                    else
                        this.tryExecutePipe(outs[i][0], outs[i][1])
                }
            }else
                for (let i in aNexts)
                    this.tryExecutePipe(i, aStream)
    }
    setAspect(aTarget, aAspect){
        let ret = aTarget
        if (!ret)
            ret = ""
        if (ret.indexOf(aAspect) < 0){
            if (ret != "")
                ret += ";"
            ret += aAspect
        }
        return ret
    }
    removeAspect(aType, aAspect){

        function doRemoveAspect(aOriAspect){
            if (aOriAspect){
                let idx = aOriAspect.indexOf(aAspect)
                if (idx > 0)
                    aOriAspect = aOriAspect.splice(idx - 1, aAspect.length)
                else if (!idx)
                    aOriAspect = aOriAspect.splice(idx, aAspect.length)
            }
            return aOriAspect
        }

        if (aType == "before"){
            if (!aAspect)
                this.m_before = null
            else
                this.m_before = doRemoveAspect(this.m_before)
        }else if (aType == "around"){
            if (!aAspect)
                this.m_around = null
            else
                this.m_around = doRemoveAspect(this.m_around)
        }else if (aType == "after"){
            if (!aAspect)
                this.m_after = null
            else
                this.m_after = doRemoveAspect(this.m_after)
        }
    }
    doAspect(aName, aStream){
        if (!aName)
            return true
        let ret = false
        let nms = aName.split(";")
        for (let i in nms){
            let pip = this.m_parent.m_pipes[nms[i]]
            if (pip){
                pip.doEvent(aStream)
                if (aStream.m_outs)
                    ret = true
            }
        }
        return ret
    }
}

class pipeFuture0 extends pipe{
    
    constructor(aParent, aName){
        super(aParent, aName)
        this.m_next2 = []
    }

    insertNext(aName, aTag){
        this.m_next2.push([aName, aTag])
    }
}

class pipeFuture extends pipe{

    constructor(aParent, aName){
        super(aParent)
        this.m_act_name = aName
        this.m_next2 = []
        if (this.m_parent.find(aName + "_pipe_add", false)){
            const pip = new pipeFuture0(this.m_parent, aName)
            pip.inPool(false)
            this.m_parent.call(aName + "_pipe_add")
            for (let i in pip.m_next2)
                this.insertNext(pip.m_next2[i][0], pip.m_next2[i][1])
            this.m_external = pip.m_external
            if (pip.m_before)
                this.m_before = this.setAspect(this.m_before, pip.m_before)
            if (pip.m_around)
                this.m_around = this.setAspect(this.m_around, pip.m_around)
            if (pip.m_after)
                this.m_after = this.setAspect(this.m_after, pip.m_after)
            delete this.m_parent.m_pipes[aName]
        }

        this.m_parent.add(e => {
            const pip = this.m_parent.find(aName, false)
            for (let i in this.m_next2)
                pip.insertNext(this.m_next2[i][0], this.m_next2[i][1])
            pip.m_external = this.m_external
            if (this.m_before)
                pip.m_before = pip.setAspect(pip.m_before, this.m_before)
            if (this.m_around)
                pip.m_around = pip.setAspect(pip.m_around, this.m_around)
            if (this.m_after)
                pip.m_after = pip.setAspect(pip.m_after, this.m_after)
            delete this.m_parent.m_pipes[this.m_name]
        }, {name: aName + "_pipe_add"})
    }

    resetTopo(){
        this.m_next2 = []
    }

    actName(){
        return this.m_act_name
    }

    removeNext(aName, aAndDelete = false, aOutside = true){
        for (let i = this.m_next2.length - 1; i >= 0; --i){
            if (this.m_next2[i][0] == aName)
                delete this.m_next2[i]
        }
        if (aAndDelete)
            this.m_parent.remove(aName, aOutside)
    }

    insertNext(aName, aTag){
        this.m_next2.push([aName, aTag])
    }

    execute(aStream){
        let sync = {}
        if (this.m_next2.length)
            sync["next"] = this.m_next2
        if (this.m_before)
            sync["before"] = this.m_before
        if (this.m_around)
            sync["around"] = this.m_around
        if (this.m_after)
            sync["after"] = this.m_after
        this.m_parent.tryExecutePipeOutside(this.actName(), aStream, sync, "any")
    }
}

var pipeline_s = {}
function pipelines(aName = "js"){
    if (!pipeline_s[aName])
        if (aName == "js")
            pipeline_s[aName] = new pipeline(aName)
        else{
            pipeline_s[aName] = pipelines().call("create" + aName + "pipeline").data()
        }

    return pipeline_s[aName]
}

class pipeline{

    constructor(aName){
        this.m_name = aName
        this.m_pipes = {}

        if (aName == "js"){
            this.add(aInput => {
                console.log("js_pipe_count: " + Object.keys(this.m_pipes).length)
                aInput.out()
            }, {name: "reportJSLeak"})
        }
    }

    name(){
        return this.m_name
    }

    add(aFunc, aParam = {}){
        const nm = aParam["name"]
        let pip
        if (aParam["type"])
            pip = pipelines().call("createJSPipe" + aParam["type"], "", new scopeCache({"parent": this, "name": nm})).data()
        else
            pip = new pipe(this, nm)
        pip.inPool(aParam["replace"])
        if (nm != ""){
            const ad = pip.actName() + "_pipe_add"
            if (this.m_pipes[ad]){
                this.call(ad)
                delete this.m_pipes[ad]
            }
        }
        pip.initialize(aFunc, aParam)

        let bf = aParam["before"]
        if (bf){
            let joint = this.find(bf)
            joint.m_before = joint.setAspect(joint.m_before, pip.actName())
        }
        let ar = aParam["around"]
        if (ar){
            let joint = this.find(ar)
            joint.m_around = joint.setAspect(joint.m_around, pip.actName())
        }
        let af = aParam["after"]
        if (af){
            let joint = this.find(af)
            joint.m_after = joint.setAspect(joint.m_after, pip.actName())
        }

        return pip
    }

    remove(aName, aOutside = false){
        let pip = this.m_pipes[aName]
        if (pip)
            delete this.m_pipes[aName]
        else{
            pip = this.find(aName + "_pipe_add", false);
            if (pip){
                pip = new pipeFuture0(this, aName);
                pip.inPool(false)
                this.call(aName + "_pipe_add");
                this.remove(aName + "_pipe_add", false);
                this.remove(aName, false);
            }
        }

        if (aOutside)
            this.removePipeOutside(aName)
    }

    find(aName, aNeedFuture = true){
        let pip = this.m_pipes[aName]
        if (!pip && aNeedFuture){
            pip = new pipeFuture(this, aName)
            pip.inPool(false)
        }
        return pip
    }

    run(aName, aInput, aTag = "", aScope = null){
        let stm = new stream(aInput, aTag, aScope)
        this.execute(aName, stm, {}, false, this.name())
        return stm
    }

    call(aName, aInput, aScope = null, aAOP = true){
        const pip = this.m_pipes[aName]
        const stm = new stream(aInput, "", aScope)
        if (pip){
            if (aAOP)
                pip.doEvent(stm)
            else
                pip.m_func(stm)
        }
        return stm
    }

    externalNextGot(aPipe, aStream, aFrom){
        if (aPipe.m_external != this.name() && aPipe.m_external != aFrom){
            this.tryExecutePipeOutside(aPipe.actName(), aStream, {}, aPipe.m_external);
            return false;
        }
        return true;
    }

    execute(aName, aStream, aSync, aFutureNeed = false, aFrom = ""){
        let pip = this.find(aName, !aFutureNeed)
        if (!pip)
            return
        if (aFrom != "" && !this.externalNextGot(pip, aStream, aFrom))
            return;
        pip = this.find(aName)
        if (aSync){
            if (Object.keys(aSync).length > 0)
                pip.resetTopo()
            if (aSync["next"])
                for (let i in aSync["next"])
                    pip.insertNext(aSync["next"][i][0], aSync["next"][i][1])
            if (aSync["before"])
                pip.m_before = pip.setAspect(pip.m_before, aSync["before"])
            if (aSync["after"])
                pip.m_after = pip.setAspect(pip.m_after, aSync["after"])
            if (aSync["around"])
                pip.m_around = pip.setAspect(pip.m_around, aSync["around"])
        }
        pip.execute(aStream)
    }

    input(aInput, aTag = "", aScope = null, aAutoTag = false){
        const tag = (aAutoTag && aTag == "") ? generateUUID() : aTag
        return new stream(aInput, tag, aScope)
    }

    tryExecutePipeOutside(aName, aStream, aSync, aFlag){
        for (let i in pipeline_s){
            if (pipeline_s[i] != this){
                if (aFlag == "any")
                    pipeline_s[i].execute(aName, aStream, aSync, true, this.name());
                else if (aFlag == pipeline_s[i].name())
                    pipeline_s[i].execute(aName, aStream, aSync);
            }
        }
    }

    removePipeOutside(aName){
        for (let i in pipeline_s)
            if (pipeline_s[i] != this)
                pipeline_s[i].remove(aName);
    }
}

class pipePartial extends pipe{
    constructor(aParent, aName){
        super(aParent, aName)
        this.m_next2 = {}
    }

    insertNext(aName, aTag){
        if (!this.m_next2[aTag])
            this.m_next2[aTag] = {}
        this.m_next2[aTag][aName] = aTag
    }

    removeNext(aName, aAndDelete = false, aOutside = true){
        for (let i in this.m_next2)
            delete this.m_next2[i][aName]
        if (aAndDelete)
            this.m_parent.remove(aName, aOutside)
    }

    replaceTopo(aOldPipe){
        pipe.prototype.replaceTopo.call(this, aOldPipe)
        this.m_next2 = aOldPipe.m_next2
    }

    resetTopo(){
        this.m_next2 = {}
    }

    execute(aStream){
        this.doEvent(aStream)
        this.doNextEvent(this.m_next2[aStream.tag()], aStream)
    }
}

class pipeDelegate extends pipe{

    constructor(aParent, aName){
        super(aParent, aName)
        this.m_next2 = []
    }

    next(aNext, aTag = ""){
        this.m_parent.find(this.m_delegate).next(aNext, aTag)
    }
    removeNext(aName, aAndDelete = false, aOutside = true){
        this.m_parent.find(this.m_delegate).removeNext(aName, aAndDelete, aOutside)
    }
    insertNext(aName, aTag){
        this.m_next2.push([aName, aTag])
    }
    replaceTopo(aOldPipe){
        pipe.prototype.replaceTopo.call(this, aOldPipe)
        this.m_next2 = aOldPipe.m_next2
    }
    resetTopo(){
        this.m_next2 = []
        this.m_parent.find(this.m_delegate).resetTopo()
    }
    execute(aStream){
        this.doEvent(aStream)
    }
    initialize(aFunc, aParam){
        this.m_delegate = aParam["delegate"]
        const del = this.m_parent.find(this.m_delegate)
        for (let i in this.m_next2)
            del.insertNext(this.m_next2[i][0], this.m_next2[i][1])
        pipe.prototype.initialize.call(this, aFunc, aParam)
    }
}

pipelines().add(function(aInput){
    const sp = aInput.scope()
    aInput.setData(new pipePartial(sp.data("parent"), sp.data("name")))
}, {name: "createJSPipePartial"})

pipelines().add(function(aInput){
    const sp = aInput.scope()
    aInput.setData(new pipeDelegate(sp.data("parent"), sp.data("name")))
}, {name: "createJSPipeDelegate"})

//#endregion

//#region Pipeline Linker
let webc = null
if (typeof module === 'object') 
    webc = require("./qwebchannel").QWebChannel
else if (typeof QWebChannel === 'function')
    webc = QWebChannel

let candidates_functions0 = []
let web_channel = null
function initLinker(aFunc){
    if (web_channel)
        aFunc(web_channel)
    else{
        if (!candidates_functions0.length){
            if (typeof qt == "undefined"){
                console.log("qt object not exists!")
                return
            }
            if (!webc){
                console.log("no qwebChannel.js!")
                return
            }
            new webc(qt.webChannelTransport, channel=>{
                for (let i in candidates_functions0)
                    candidates_functions0[i](channel)
                candidates_functions0 = []
                web_channel = channel
            })
        }
        candidates_functions0.push(aFunc)
    }
}

let environment = {}
function getEnv(){
    return environment
}

class pipelineOutside extends pipeline{

    constructor(aName){
        super(aName)
        this.candidates_functions = []
    }

    init(aFunc){
        if (!web_channel){
            if (!this.candidates_functions.length){
                initLinker(channel=>{
                    this.Linker = channel.objects["Pipeline" + this.name()]
                    if (!this.Linker){
                        console.log("no " + this.name() + " linker")
                        return
                    }
                    if (channel.objects["Environment"])
                        environment = channel.objects["Environment"].env
                    this.Linker.executeJSPipe.connect(function(aName, aData, aTag, aScope, aSync, aFutureNeed, aFrom){
                        const len = Object.keys(aScope).length
                        let sp = {}
                        for (let i = 0; i < len; i += 2)
                            sp[aScope[i]] = aScope[i + 1]
                        pipelines().execute(aName, new stream(aData, aTag, new scopeCache(sp)), aSync, aFutureNeed, aFrom)
                    })
                    this.Linker.removeJSPipe.connect(function(aName){
                        pipelines().remove(aName)
                    })

                    for (let i in this.candidates_functions)
                        this.candidates_functions[i]()
                    this.candidates_functions = []
                })
            }
            this.candidates_functions.push(aFunc)
        }else{
            aFunc()
        }
    }

    execute(aName, aStream, aSync, aFutureNeed = false, aFlag = ""){
        this.init(e=>{
            if (this.Linker)
                this.Linker.executeFromJS(aName, aStream.data(), aStream.tag(), aStream.scope().m_data, aSync, aFutureNeed, aFlag)
        })
    }

    remove(aName, aOutside = false){
        this.init(e=>{
            if (this.Linker)
                this.Linker.removeFromJS(aName)
        })
    }
}

//#endregion

class pipelineCPlus extends pipelineOutside{
    constructor(){
        super("c++")
    }
}

pipelines().add(function(aInput){
    aInput.setData(new pipelineCPlus())
}, {name: "createc++pipeline"})

pipelines("c++").init(function(){})

class pipelineQML extends pipelineOutside{
    constructor(){
        super("qml")
    }
}

pipelines().add(function(aInput){
    aInput.setData(new pipelineQML())
}, {name: "createqmlpipeline"})

pipelines("qml").init(function(){})

if (typeof module == "object")
    module.exports = {
        pipelines: pipelines,
        scopeCache: scopeCache,
        env: getEnv
    }
