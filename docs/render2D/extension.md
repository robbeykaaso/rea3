# Abstract
* qsgObject permits the developer to customize his own qsgObject type  
_sample_:  
```
class polyObjectEx : public rea::polyObject{
public:
    polyObjectEx(const QJsonObject& aConfig) : polyObject(aConfig){

    }
    bool bePointSelected(double aX, double aY) override{
        return shapeObject::bePointSelected(aX, aY) && pointIsIn(m_points, Point_2(aX, aY));
    }
};

rea::pipeline::instance()->add<QJsonObject, rea::pipePartial>([](rea::stream<QJsonObject>* aInput){
    aInput->scope()->cache<std::shared_ptr<rea::qsgObject>>("result", std::make_shared<polyObjectEx>(aInput->data()));
    aInput->out();
}, rea::Json("around", "create_qsgobject_poly")); 
```