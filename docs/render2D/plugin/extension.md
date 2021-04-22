# Abstract  
* qsgBoardPlugin permits the developer to customize his own control plugin  
_sample_:
```
class qsgPluginTransform : public qsgBoardPlugin{
public:
    qsgPluginTransform(const QJsonObject& aConfig) : qsgBoardPlugin(aConfig){}
    ~qsgPluginTransform() override;
}

static rea::regPip<QJsonObject, rea::pipePartial> create_qsgboardplugin_transform([](rea::stream<QJsonObject>* aInput){
    aInput->scope()->cache<std::shared_ptr<qsgBoardPlugin>>("result", std::make_shared<qsgPluginTransform>(aInput->data()));
    aInput->out();
}, rea::Json("name", "create_qsgboardplugin_transform"));  //we can use "after" aspect to reuse the old control plugin's logic
```