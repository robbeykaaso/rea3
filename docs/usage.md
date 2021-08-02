# Abstract
this page describe how to use the framework in your codes  

# Steps
**0**: install qt library  

**1**: import rea library and initialize it if necessary    
_sample_:
```
    //c++/qml end; initialize the engine only once for c++/qml
    QQmlApplicationEngine engine;
    rea::pipeline::instance()->run<QQmlApplicationEngine*>("initRea", &engine);

    //js end
    npm install reajs
```    
</br>

#### I. Lazy running(reactive program)

**2**: add pipes and build the pipeline graph by connecting pipes  
_sample_:
```
    //c++
    #include "rea.h"
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        aInput->setData(dst::Json("hello", "world"))  //set the data of the stream
              ->out(); //output the stream and run the nexts after this pipe
    }, Json("name", "pipe0", "thread", 2))  //add pipe0
    .next("pipe1")  //connect pipe1 after pipe0

    //qml
    import Pipeline 1.0
    Pipeline.add(function(aInput){  
        aInput.setData("").out()  
    }, {name: "pipe0"})
    .next("pipe1")

    //js
    //var rea = require('reajs')  //es5, common.js
    import rea from 'reajs'  //es6
    rea.pipelines().add(function(aInput){  
        aInput.setData("").out()  
    }, {name: "pipe0"})
    .next("pipe1")
```  
</br>

**3**: run the pipeline  
_sample_:  
```
    //c++
    pipeline::instance()->run<QJsonObject>("pipe0", QJsonObject())  //run the pipeline: pipe0 -> pipe1

    //qml
    Pipeline.run("pipe0", {})

    //js
    rea.pipelines().run("pipe0", {})
```  
</br>

#### II. Instant running(functional program)
**2**: write your codes as a stream style or a normal coding style  
_sample_:  
```
    //c++
    rea::pipeline::instance()->input<int>(0, "test")
    ->asyncCallF<int>([](rea::stream<int>* aInput){
        aInput->outs("world")
    }, rea::Json("thread", 1))
    ->asyncCall<QString>("doSomething")
    ->asyncCallF<QString>([](rea::stream<QString>* aInput){
        assert(aInput->data() == "world");
        aInput->setData("Pass")->out();
    })
    ->asyncCall("success");
    //c++
    auto dt = rea::pipeline::instance()->asyncCall<QJsonArray>("doSomething", QJsonArray())->data()

    //qml
    Pipeline.input(0, "test")
    .asyncCallF(function(aInput){
        aInput.outs("world")
    })
    .asyncCall("doSomething")
    .asyncCallF(function(aInput){
        console.assert(aInput.data() == "world")
        aInput.setData("Pass").out()
    })
    .asyncCall("success")
    //qml
    var dt = Pipeline.asyncCall("doSomething", []).data()
    //qml
    gc()
    
    //js
    await pipelines().input(0, "test")
          .asyncCallS([
             function(aInput){
                aInput.outs("world")
            },
            "doSomething",
            function(aInput){
                console.assert(aInput.data() == "world")
                aInput.setData("Pass").out()
            },
            "success"
          ])
    //js
    let stm = await pipelines().input([]).asyncCall("doSomething")
    let dt = stm.data()
```

#### III. Function extending(aspect oriented program)
**2**: extend your codes by pipe configs  
_sample_:  
```  
    //c++//other language is like the same
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something original version
    }, Json("name", "pipe0"))  //add pipe0
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something before original version
    }, Json("before", "pipe0"))  //inject logic before 
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something after original version
    }, Json("after", "pipe0"))  //inject logic after
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something before original version
        pipeline::instance()->call("pipe0", aInput->data(), aInput->scope(), true);
        //do something after original version
    }, Json("around", "pipe0"))  //inject logic around  
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something
    }, Json("befored", "pipe0"))  //inject "pipe0" before this pipe
    pipeline::instance()->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something
    }, Json("aftered", "pipe0"))  //inject "pipe0" after this pipe 
```

#### IV. Cascade connection(service mesh)
**2**: initialize pipeline linker  
_sample_:
```
//c++ server end
static rea::regPip<std::shared_ptr<rea::pipeline*>> reg_create_c_client_pipeline([](rea::stream<std::shared_ptr<rea::pipeline*>>* aInput){
    *aInput->data() = new rea::pipelineRemote("qml", "server");
}, rea::Json("name", "createqmlpipeline"));

static rea::regPip<QQmlApplicationEngine*> reg_tcp_linker([](rea::stream<QQmlApplicationEngine*>* aInput){
    static normalServer server;
    auto write_remote = [](rea::stream<QJsonObject>* aInput){
        server.writeSocket(aInput->scope()->data<QTcpSocket*>("socket"), aInput->data());
    };
    rea::connectRemote("server", "qml", write_remote, false, "qml_server");
    aInput->out();
}, rea::Json("name", "install0_tcp"), "initRea");

//c++ client end
static rea::regPip<std::shared_ptr<rea::pipeline*>> reg_create_c_server_pipeline([](rea::stream<std::shared_ptr<rea::pipeline*>>* aInput){
    *aInput->data() = new rea::pipelineRemote("qml_server", "qml");
}, rea::Json("name", "createqml_serverpipeline"));

static rea::regPip<QQmlApplicationEngine*> reg_tcp_linker([](rea::stream<QQmlApplicationEngine*>* aInput){
    static normalClient client;
    auto write_remote = [](rea::stream<QJsonObject>* aInput){
        client.sendServer(aInput);
    };
    rea::connectRemote("qml", "qml_server", write_remote);
    aInput->out();
}, rea::Json("name", "install0_tcp"), "initRea");

//and so on
```  
</br>

**3**: code as above  
_sample_:
```
    //c++
    pipeline::instance("c++")->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something in c++
        aInput.out();
    }, Json("name", "pipe0",
            "external", "qml"))
    //js
    pipelines("js").add(function(aInput){
        //do something in js
        aInput.out()
    }, {name: "pipe1",
        external: "qml"})
    //c++ on the another process
    pipeline::instance("remote")->add<QJsonObject>(stream<QJsonObject>* aInput){
        //do something in c++
        aInput.out();
    }, Json("name", "pipe2",
            "external", "qml"))
    //build the service in qml
    Pipeline.find("pipe0")
            .next("pipe1")
            .next("pipe2")
            .nextF(function(aInput){
                //do something in qml
                aInput.out()
            })
```

# Notice  
* <font color="red">each named pipe may be on different code environment. if you use the gramma with rea, you don't care the detail of them</font><br />  