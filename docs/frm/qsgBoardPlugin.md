# Abstract
a super class for customized qsgBoardPlugin. it provides all the manual mouse events for differet business

# Usage
the provided plugins includes: "transform", "select", "drawfree", "drawRect", "drawEllipse", "editNode"  
_sample_:  
```
//register
static rea::regPip<QJsonObject, rea::pipePartial> create_qsgboardplugin_transform([](rea::stream<QJsonObject>* aInput){
   aInput->out<std::shared_ptr<qsgBoardPlugin>>(std::make_shared<qsgPluginTransform>(aInput->data()));
}, rea::Json("name", "create_qsgboardplugin_transform"));  //transform is the plugin type

//install
plugins: [{type: "transform"}]  //install it as a plugin of qsgBoard
```  
</br>