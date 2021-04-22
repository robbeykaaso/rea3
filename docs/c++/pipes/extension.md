# Abstract
* rea-c++ provides some common kinds of pipes, however there also be many common logics coupled with app's own business logic such as buffer, throttle and so on. rea-c++ permit the developer to customize his own pipe type  
_sample_:  
```
template <typename T, typename F>
class pipeCustomQML : public rea::pipe<T, F> {
public:
    pipeCustomQML(rea::pipeline* aParent, const QString& aName, int aThreadNo = 0, bool aReplace = false) : rea::pipe<T, F>(aParent, aName, aThreadNo, aReplace) {

    }
protected:
    bool event( QEvent* e) override{
        if(e->type()== rea::pipe0::streamEvent::type){
            auto eve = reinterpret_cast<rea::pipe0::streamEvent*>(e);
            if (eve->getName() == rea::pipe0::m_name){
                auto stm = std::dynamic_pointer_cast<rea::stream<T>>(eve->getStream());
                stm->scope()->template cache<QString>("flag", "test48");
                doEvent(stm);
                doNextEvent(rea::pipe0::m_next, stm);
            }
        }
        return true;
    }
};

```

# Reference
[pipe](../pipe.md)