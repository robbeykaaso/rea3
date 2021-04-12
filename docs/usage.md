# Abstract
this page describe how to use the framework in your codes  

# Steps
**0**: initialize the reactive engine only once  
_sample_:
```
    QQmlApplicationEngine engine;
    pipeline::run<QQmlApplicationEngine*>("regQML", &engine);
```    
</br>

#### I. Lazy running

**1**: add pipes and build the pipeline graph by connecting pipes  
_sample_:
```
    pipeline::add<QJsonObject>(stream<QJsonObject>* aInput){
        aInput->setData(dst::Json("hello", "world"));  //set the data of the stream
        aInput->out(); //output the stream and run the nexts after this pipe
    }, Json("name", "pipe0", "thread", 2, "module", "module1"))  //add pipe0 in c++
    .next("pipe1")

    Pipeline.add(function(aInput){  //add a pipe1 and connect it after the pipe0
        return {data: aInput, out: {}} //output the stream and run the nexts after this pipe
    }, {name: "pipe1", "module", "module1"})
```  
</br>

**2**: run the pipeline  
_sample_:  
```
    pipeline::run<QJsonObject>("pipe0", QJsonObject())  //run the pipeline: pipe0 -> pipe1 in c++

    Pipeline.run("pipe0") //run the pipeline: pipe0 -> pipe1 in qml
```  
</br>

#### II. Instant running

**1**: write your codes as a stream style or a normal coding style  
_sample_:  
```
    ////c++ coding
    rea::pipeline::input<int>(0, "test12")  //code style like ranges
    ->call<int>([](rea::stream<int>* aInput){
        aInput->setData(aInput->data() + 1)->out();
    }, rea::Json("thread", 1))
    ->call<QString>("doSomething")
    ->call<QString>([](rea::stream<QString>* aInput){
        assert(aInput->data() == "world");
        aInput->setData("Pass: test12")->out();
    })
    ->call("testSuccess");

    //do something
    auto dt = rea::pipeline::call<QJsonArray>("project_label_listViewSelected", QJsonArray())->data()  //code style like await in es6
    //do something

    ////qml coding
    Pipeline.input("testFS2.json", "testQMLStg3", true, {"testFS2.json": {hello: "world"}})
    .call("writeJson2")
    .call(function(aInput){
        var dt = aInput.varData("testFS2.json", "object")
        console.assert(dt["hello"] === "world")
        aInput.var("testFS2.json", {hello: "world2"}).out()
    })
    .call("readJson2")
    .call("deletePath")
    .call(function(aInput){
        var dt = aInput.varData("testFS2.json", "object")
        console.assert(dt["hello"] === "world")
        aInput.setData("Pass: testQMLStorage3 ").out()
    })
    .call("testSuccess")
    .destroy()  //explicitly destroy the qml stream to trig the end of the transaction

    //do something
    var local_scope = function(){ 
        return Pipeline.call("service1", {}).data()
    }
    var dt = local_scope()
    gc()  //explicitly destroy the qml stream to trig the end of the transaction
    //do something
```