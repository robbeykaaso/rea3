# Abstract
* a super class for customized qsgBoardPlugin. it provides all the manual mouse events for differet business  

# Usage
* the provided plugins includes: "transform", "select", "drawfree", "drawRect", "drawEllipse", "editNode" and so on  
_sample_:  
```
//register
static rea::regPip<QJsonObject, rea::pipePartial> create_qsgboardplugin_transform([](rea::stream<QJsonObject>* aInput){
   aInput->scope()->cache<std::shared_ptr<qsgBoardPlugin>>("result", std::make_shared<qsgPluginTransform>(aInput->data()));
   aInput->out();
}, rea::Json("name", "create_qsgboardplugin_transform"));  //transform is the plugin type

//install
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "select")})  //switch to select mode
plugins: [{type: "transform"}]  //install the transform plugin for the qsgBoard
```  
</br>

# Reference
[qsgBoard](qsgBoard.md)  