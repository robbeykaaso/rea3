#ifndef REA_H_
#define REA_H_

#include "util.h"
#include <vector>
#include <functional>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QEvent>
#include <QEventLoop>
#include <QCoreApplication>

namespace rea {

class pipeline;

class stream0;
template <typename T>
class stream;
template <typename T>
using pipeFunc = std::function<void(stream<T>*)>;

template <typename T, typename F = pipeFunc<T>>
class pipe;

class DSTDLL scopeCache : public std::enable_shared_from_this<scopeCache>{
public:
    scopeCache(){
    }
    scopeCache(const QJsonObject& aData);
    scopeCache(scopeCache&&) = default;
    ~scopeCache();
    template<typename T>
    std::shared_ptr<scopeCache> cache(const QString& aName, T aData){
        m_data.insert(aName, in(aData));
        return shared_from_this();
    }
    template<typename T>
    T data(const QString& aName){
        if (m_data.contains(aName))
            return std::dynamic_pointer_cast<stream<T>>(m_data.value(aName))->data();
        else
            return T();
    }
public:
    QVariantList toList();
    std::shared_ptr<stream0> dataStream(const QString& aName){
        return m_data.value(aName);
    }
private:
    QHash<QString, std::shared_ptr<stream0>> m_data;
    friend stream0;
};

class DSTDLL stream0 : public std::enable_shared_from_this<stream0>{
public:
    stream0(const QString& aTag = "");
    stream0(const stream0&) = default;
    stream0(stream0&&) = default;
    stream0& operator=(const stream0&) = default;
    stream0& operator=(stream0&&) = default;
    virtual ~stream0();
    QString tag(){
        return m_tag;
    }
    std::shared_ptr<scopeCache> scope(bool aNew = false){
        if (!m_scope || aNew)
            m_scope = std::make_shared<scopeCache>();
        return m_scope;
    }
public:
    virtual bool supportedType(){return false;}
    virtual QVariant QData(){
        return QVariant();
    }
protected:
    //std::shared_ptr<QEventLoop> waitLastAsync(const QString& aName);
   // void freeAsync(const QString& aName);
    pipeline* m_parent;
    QString m_tag;
    std::shared_ptr<scopeCache> m_scope;
    std::shared_ptr<std::vector<std::pair<QString, std::shared_ptr<stream0>>>> m_outs = nullptr;
    friend class pipe0;
    friend pipeline;
    template<typename T, typename F>
    friend class pipe;
    template <typename T>
    friend class stream;
};

class pipeFuture;
template <typename T, typename F = pipeFunc<T>>
class pipeDelegate;
template <typename T, typename F>
class pipeParallel;

class DSTDLL pipe0 : public QObject{
public:
    enum AspectType {AspectBefore, AspectAround, AspectAfter};
public:
    virtual ~pipe0();
    virtual QString actName() {return m_name;}

    template <typename T>
    pipe0* nextF(pipeFunc<T> aNextFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject()){
        return nextF0(m_parent, this, aNextFunc, aTag, aParam);
    }
    virtual pipe0* nextP(pipe0* aNext, const QString& aTag = "");
    virtual pipe0* next(const QString& aName, const QString& aTag = "");

    template <typename T>
    pipe0* nextFB(pipeFunc<T> aNextFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject()){
        nextF<T>(aNextFunc, aTag, aParam);
        return this;
    }
    virtual pipe0* nextPB(pipe0* aNext, const QString& aTag = "");
    virtual pipe0* nextB(const QString& aName, const QString& aTag = "");

    virtual void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true);
    void removeAspect(pipe0::AspectType aType, const QString& aAspect = "");
public:
    virtual void execute(std::shared_ptr<stream0> aStream);
    virtual void resetTopo();
private:
    friend pipeline;
protected:
    class streamEvent : public QEvent{
    public:
        static const Type type = static_cast<Type>(QEvent::User + 1);
    public:
        streamEvent(const QString& aName, std::shared_ptr<stream0> aStream) : QEvent(type) {
            m_name = aName;
            m_stream = aStream;
        }
        QString getName() {return m_name;}
        std::shared_ptr<stream0> getStream() {return m_stream;}
    private:
        QString m_name;
        std::shared_ptr<stream0> m_stream;
    };

    pipe0(pipeline* aParent, const QString& aName = "", int aThreadNo = 0);
    void inPool(bool aReplace);
    virtual void replaceTopo(pipe0* aOldPipe);
    virtual void insertNext(const QString& aName, const QString& aTag) {
        m_next.insert(aName, aTag);
    }
protected:
    void doNextEvent(const QMap<QString, QString>& aNexts, std::shared_ptr<stream0> aStream);
    void setAspect(QString& aTarget, const QString& aAspect);
protected:
    QString m_name;
    QMap<QString, QString> m_next;
    QString m_before = "", m_around = "", m_after = "";
    QString m_external = "";
    pipeline* m_parent;
    QThread* m_thread = QThread::currentThread();
private:
    friend pipeFuture;
    void tryExecutePipe(const QString& aName, std::shared_ptr<stream0> aStream);
    template<typename, typename>
    friend class pipeDelegate;
};

class pipeFuture : public pipe0 {
public:
    QString actName() override {return m_act_name;}
    void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true) override;
    void resetTopo() override;
    void execute(std::shared_ptr<stream0> aStream) override;
protected: 
    pipeFuture(pipeline* aParent, const QString& aName);
    ~pipeFuture() override;
    bool event( QEvent* e) override;
    void insertNext(const QString& aName, const QString& aTag) override;
private:
    QString m_act_name;
    QVector<QPair<QString, QString>> m_next2;
    friend pipeline;
};

template<typename T>
inline std::shared_ptr<stream<T>> in(T aInput = T(), const QString& aTag = "", std::shared_ptr<scopeCache> aScope = nullptr, bool aAutoTag = false){
    auto tag = (aAutoTag && aTag == "") ? rea::generateUUID() : aTag;
    return std::make_shared<stream<T>>(aInput, tag, aScope);
}

template<typename T, typename F>
class funcType{
public:
    void doEvent(F aFunc, std::shared_ptr<stream<T>> aStream){
        if (aFunc)
            aFunc(aStream.get());
    }
};

class DSTDLL pipeline : public QObject{
public:
    static pipeline* instance(const QString& aName = getDefaultPipelineName());
public:
    pipeline(const QString& aName = getDefaultPipelineName());
    pipeline(pipeline&&) = delete;
    QString name(){return m_name;}
    virtual ~pipeline();

    virtual void remove(const QString& aName, bool aOutside = false);

    template<typename T, template<class, typename> class P = pipe, typename F = pipeFunc<T>, typename S = pipeFunc<T>>
    pipe0* add(F aFunc, const QJsonObject& aParam = QJsonObject()){
        auto nm = aParam.value("name").toString();
        auto tmp = new P<T, S>(this, nm, aParam.value("thread").toInt());  //https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters
        tmp->inPool(aParam.value("replace").toBool());
        if (nm != ""){
            auto ad = tmp->actName() + "_pipe_add";
            if (m_pipes.contains(ad)){
                call<int>(ad + "0", 0);
                doRemove(ad);
            }
        }
        tmp->initialize(aFunc, aParam);
        pipe0* ret = tmp;

        auto bf = aParam.value("before").toString();
        if (bf != ""){
            auto joint = find(bf);
            joint->setAspect(joint->m_before, ret->actName());
        }
        auto ar = aParam.value("around").toString();
        if (ar != ""){
            auto joint = find(ar);
            joint->setAspect(joint->m_around, ret->actName());
        }
        auto af = aParam.value("after").toString();
        if (af != ""){
            auto joint = find(af);
            joint->setAspect(joint->m_after, ret->actName());
        }
        return ret;
    }

    pipe0* find(const QString& aName, bool aNeedFuture = true) {
        auto ret = m_pipes.value(aName);
        if (!ret && aNeedFuture){
            auto f = aName + "_pipe_add";
            if (m_pipes.contains(f))
                ret = m_pipes.value(f);
            else{
                ret = new pipeFuture(this, aName);
                ret->inPool(false);
            }
        }
        return ret;
    }

    template<typename T>
    std::shared_ptr<stream<T>> run(const QString& aName, T aInput = T(), const QString& aTag = "", std::shared_ptr<scopeCache> aScope = nullptr){
        auto stm = in(aInput, aTag, aScope);
        execute(aName, stm, QJsonObject(), false, name());
        return stm;
    }

    template<typename T, typename F = pipeFunc<T>>
    std::shared_ptr<stream<T>> call(const QString& aName, std::shared_ptr<stream<T>> aInput, bool aAOP = true){
        auto pip = m_pipes.value(aName);
        if (pip){
            auto pip2 = dynamic_cast<pipe<T, F>*>(pip);
            if (aAOP)
                pip2->doEvent(aInput);
            else
                funcType<T, F>().doEvent(pip2->m_func, aInput);
        }
        return aInput;
    }

    template<typename T, typename F = pipeFunc<T>>
    std::shared_ptr<stream<T>> call(const QString& aName, T aInput = T(), std::shared_ptr<scopeCache> aScope = nullptr, bool aAOP = true){
        auto stm = in(aInput, "", aScope);
        return call(aName, stm, aAOP);
    }

    template<typename T>
    std::shared_ptr<stream<T>> asyncCall(const QString& aName, T aInput = T(), bool aEventLevel = true, bool aOutside = false){
        return in<T>(aInput)->asyncCall(aName, aEventLevel, this->name(), aOutside);
    }

    template<typename T>
    void supportType(std::function<QVariant(stream0*)> aTransfer){
        m_types.insert(typeid (T).name(), aTransfer);
    }

    template<typename T>
    static std::shared_ptr<stream<T>> input(T aInput = T(), const QString& aTag = "", std::shared_ptr<scopeCache> aScope = nullptr, bool aAutoTag = false){
        return in(aInput, aTag, aScope, aAutoTag);
    }

    virtual void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(),
                         bool aFutureNeed = false, const QString& aFrom = "");

    void updateOutsideRanges(const QSet<QString>& aRanges);
protected:
    virtual void removePipeOutside(const QString& aName);
    virtual void tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag);
    virtual bool externalNextGot(pipe0* aPipe, std::shared_ptr<stream0> aStream, const QString& aFrom);
    QHash<QString, pipe0*> m_pipes;
private:
    QThread* findThread(int aNo);
    void doRemove(const QString& aName);
    QString m_name;
    QHash<int, QThread*> m_threads;
    QHash<QString, std::function<QVariant(stream0*)>> m_types;
    QSet<QString> m_outside_pipelines;
    friend pipe0;
    friend pipeFuture;
    friend stream0;
    template<typename T, typename F>
    friend class pipe;
    template<typename T>
    friend class stream;
};

template <typename T>
class stream : public stream0{
public:
    stream() : stream0(){}
    stream(T aInput, const QString& aTag = "", std::shared_ptr<scopeCache> aScope = nullptr) : stream0(aTag){
        m_data = aInput;
        m_scope = aScope;
    }
public:
    stream<T>* setData(T aData) {
        m_data = aData;
        return this;
    }
    T data() {return m_data;}

    stream<T>* out(const QString& aTag = ""){
        if (!m_outs)
            m_outs = std::make_shared<std::vector<std::pair<QString, std::shared_ptr<stream0>>>>();
        if (aTag != "")
            m_tag = aTag;
        return this;
    }

    void noOut(){
        m_outs = nullptr;
    }

    template<typename S>
    stream<S>* outs(S aOut = S(), const QString& aNext = "", const QString& aTag = ""){
        if (!m_outs)
            m_outs = std::make_shared<std::vector<std::pair<QString, std::shared_ptr<stream0>>>>();
        auto ot = in(aOut, aTag == "" ? m_tag : aTag, m_scope);
        m_outs->push_back(std::pair<QString, std::shared_ptr<stream0>>(aNext, ot));
        return ot.get();
    }

    template<typename S>
    stream<T>* outsB(S aOut = S(), const QString& aNext = "", const QString& aTag = ""){
        outs<S>(aOut, aNext, aTag);
        return this;
    }

    template<typename S = T>
    std::shared_ptr<stream<S>> asyncCall(const QString& aName, bool aEventLevel = true, const QString& aPipeline = getDefaultPipelineName(), bool aOutside = false){
        std::shared_ptr<stream<S>> ret = nullptr;
        auto line = pipeline::instance(aPipeline);
        if (aEventLevel){
            auto loop = std::make_shared<QEventLoop>(); //waitLastAsync(aName);//
            bool timeout = false;
            auto monitor = line->find(aName)->nextF<S>([&loop, &timeout, &ret, this, aName](stream<S>* aInput){
                ret = in(aInput->data(), aInput->tag(), aInput->scope());
                if (loop->isRunning()){
                    loop->quit();
                    //std::cout << "quit: " << loop->isRunning() << std::endl;
                }else
                    timeout = true;
            }, m_tag);
            line->execute(aName, shared_from_this());
            if (!timeout){
                //std::cout << aName.toStdString() << " locked" << std::endl;
                loop->exec();
                //std::cout << aName.toStdString() << " finished" << std::endl;
            }
            line->find(aName)->removeNext(monitor->actName(), true, aOutside);
            //freeAsync(aName);
        }else{
            std::promise<std::shared_ptr<stream<S>>> pr;
            auto monitor = line->find(aName)->nextF<S>([this, &pr, aName](stream<S>* aInput){
                pr.set_value(in(aInput->data(), aInput->tag(), aInput->scope()));
            }, m_tag, rea::Json("thread", 1));
            line->execute(aName, shared_from_this());
            auto ft = pr.get_future();
            std::future_status st;
            do{
                st = ft.wait_for(std::chrono::microseconds(5));
            }while(st != std::future_status::ready);
            ret = ft.get();
            line->find(aName)->removeNext(monitor->actName(), true, aOutside);
        }
        return ret; //std::dynamic_pointer_cast<stream<T>>(shared_from_this());
    }

    template<typename S = T, template<class, typename> class P = pipe, typename F = pipeFunc<T>, typename R = pipeFunc<T>>
    std::shared_ptr<stream<S>> asyncCallF(F aFunc, const QJsonObject& aParam = QJsonObject(), bool aEventLevel = true, const QString& aPipeline = getDefaultPipelineName()){
        auto line = pipeline::instance(aPipeline);
        auto pip = line->add<T, P, F, R>(aFunc, aParam);
        auto ret = asyncCall<S>(pip->actName(), aEventLevel, aPipeline);
        line->remove(pip->actName());
        return ret;
    }
public:
    QVariant QData() override{
        auto tp = typeid (T).name();
        if (pipeline::instance()->m_types.contains(tp))
            return pipeline::instance()->m_types.value(tp)(this);
        else
            return QVariant();
    }

    bool supportedType() override{
        auto tp = typeid (T).name();
        return pipeline::instance()->m_types.contains(tp);
    }
private:
    T m_data;
    template<typename T, typename F>
    friend class funcType;
    template <typename T, typename F>
    friend class pipeDelegate;
};

template <typename T>
pipe0* nextF0(pipeline* aPipeline, pipe0* aPipe, pipeFunc<T> aNextFunc, const QString& aTag, const QJsonObject& aParam){
    return aPipe->next(aPipeline->add<T>(aNextFunc, aParam)->actName(), aTag);
}

template <typename T, typename F>
class pipe : public pipe0{
protected:
    pipe(pipeline* aParent, const QString& aName = "", int aThreadNo = 0) : pipe0(aParent, aName, aThreadNo) {}
    virtual pipe0* initialize(F aFunc, const QJsonObject& aParam = QJsonObject()){
        m_func = aFunc;
        m_external = aParam.value("external").toString(m_parent->name());
        auto bf = aParam.value("befored").toString();
        if (bf != "")
            setAspect(m_before, bf);
        auto ed = aParam.value("aftered").toString();
        if (ed != "")
            setAspect(m_after, ed);
        return this;
    }
    bool event( QEvent* e) override{
        if(e->type()== streamEvent::type){
            auto eve = reinterpret_cast<streamEvent*>(e);
            if (eve->getName() == m_name){
                auto stm = std::dynamic_pointer_cast<stream<T>>(eve->getStream());
                doEvent(stm);
                doNextEvent(m_next, stm);
            }
        }
        return true;
    }
    void doEvent(const std::shared_ptr<stream<T>> aStream){
        if (doAspect(m_before, aStream)){
            if (m_around != "")
                doAspect(m_around, aStream);
            else
                funcType<T, F>().doEvent(m_func, aStream);
        }
        if (aStream->m_outs)
            doAspect(m_after, aStream);
    }
protected:
    F m_func;
    friend pipeParallel<T, F>;
    friend pipeline;
private:
    bool doAspect(const QString& aName, std::shared_ptr<stream<T>> aStream){
        if (aName == "")
            return true;
        bool ret = false;
        auto nms = aName.split(";");
        for (auto i : nms){
            auto pip = m_parent->m_pipes.value(i);
            if (pip){
                auto pip2 = dynamic_cast<pipe<T, F>*>(pip);
                pip2->doEvent(aStream);
                if (aStream->m_outs)
                    ret = true;
            }
        }
        return ret;
    }
};

template <typename T, typename F>
class pipeAsync : public pipe<T, F>{
protected:
    pipeAsync(pipeline* aParent, const QString& aName = "", int aThreadNo = 0) : pipe<T, F>(aParent, aName, aThreadNo){

    }
    void execute(std::shared_ptr<stream0> aStream) override{
        auto nxt_eve = std::make_unique<pipe0::streamEvent>(pipe0::m_name, aStream);
        QCoreApplication::postEvent(this, nxt_eve.release());
    }
    friend pipeline;
};

template <typename T, typename F>
class pipeDelegate : public pipe<T, F>{
public:
    pipe0* nextP(pipe0* aNext, const QString& aTag = "") override{
        return pipe0::m_parent->find(m_delegate)->nextP(aNext, aTag);
    }
    pipe0* next(const QString& aName, const QString& aTag = "") override{
        return pipe0::m_parent->find(m_delegate)->next(aName, aTag);
    }
    void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true) override{
        pipe0::m_parent->find(m_delegate)->removeNext(aName, aAndDelete, aOutside);
    }
    void resetTopo() override{
        m_next2.clear();
        pipe0::m_parent->find(m_delegate)->resetTopo();
    }
protected:
    pipeDelegate(pipeline* aParent, const QString& aName = "", int aThreadNo = 0) : pipe<T, F>(aParent, aName, aThreadNo) {}
    pipe0* initialize(F aFunc, const QJsonObject& aParam = QJsonObject()) override{
        m_delegate = aParam.value("delegate").toString();
        auto del = pipe0::m_parent->find(m_delegate);
        for (auto i : m_next2)
            del->insertNext(i.first, i.second);
        return pipe<T, F>::initialize(aFunc, aParam);
    }
    void insertNext(const QString& aName, const QString& aTag) override{
        m_next2.push_back(QPair<QString, QString>(aName, aTag));
    }
    void replaceTopo(pipe0* aOldPipe) override{
        pipe0::replaceTopo(aOldPipe);
        m_next2 = reinterpret_cast<pipeDelegate<T, F>*>(aOldPipe)->m_next2;
    }
    bool event( QEvent* e) override{
        if(e->type()== pipe0::streamEvent::type){
            auto eve = reinterpret_cast<pipe0::streamEvent*>(e);
            if (eve->getName() == pipe0::m_name){
                auto stm = std::dynamic_pointer_cast<stream<T>>(eve->getStream());
                doEvent(stm);
            }
        }
        return true;
    }
private:
    QString m_delegate;
    QVector<QPair<QString, QString>> m_next2;
    friend pipeline;
};

template <typename T, typename F>
class parallelTask : public QRunnable{
public:
    parallelTask(pipeParallel<T, F>* aPipe, std::shared_ptr<stream<T>> aStream) : QRunnable(){
        m_pipe = aPipe;
        m_source = aStream;
    }
    void run() override{
        m_pipe->doEvent(m_source);
        m_pipe->doNextEvent(m_pipe->m_next2.value(m_source->tag()), m_source);
    }
private:
    std::shared_ptr<stream<T>> m_source;
    pipeParallel<T, F>* m_pipe;
};

template <typename T, typename F>
class pipePartial : public pipe<T, F> {
public:
    void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true) override {
        for (auto i = m_next2.begin(); i != m_next2.end(); ++i)  //: for will not remove
            i.value().remove(aName);
        if (aAndDelete)
            pipe0::m_parent->remove(aName, aOutside);
    }
    void resetTopo() override{
        m_next2.clear();
    }
protected:
    pipePartial(pipeline* aParent, const QString& aName, int aThreadNo = 0) : pipe<T, F>(aParent, aName, aThreadNo) {

    }
    void insertNext(const QString& aName, const QString& aTag) override {
        rea::tryFind(&m_next2, aTag)->insert(aName, aTag);
    }
    void replaceTopo(pipe0* aOldPipe) override{
        pipe0::replaceTopo(aOldPipe);
        m_next2 = reinterpret_cast<pipePartial<T, F>*>(aOldPipe)->m_next2;
    }
    bool event( QEvent* e) override{
        if(e->type()== pipe0::streamEvent::type){
            auto eve = reinterpret_cast<pipe0::streamEvent*>(e);
            if (eve->getName() == pipe0::m_name){
                auto stm = std::dynamic_pointer_cast<stream<T>>(eve->getStream());
                doEvent(stm);
                doNextEvent(m_next2.value(stm->tag()), stm);
            }
        }
        return true;
    }
private:
    QHash<QString, QMap<QString, QString>> m_next2;
    friend pipeline;
    friend parallelTask<T, F>;
};

template <typename T, typename F = pipeFunc<T>>
class pipeParallel : public pipePartial<T, F> {
protected:
    pipeParallel(pipeline* aParent, const QString& aName, int aThreadNo = 0) : pipePartial<T, F>(aParent, aName, aThreadNo) {

    }
    ~pipeParallel() override{

    }
    pipe0* initialize(F aFunc, const QJsonObject& aParam = QJsonObject()) override {
        m_act_name = aParam.value("delegate").toString();
        m_init = aFunc != nullptr;
        return pipe<T, F>::initialize(aFunc, aParam);
    }
    bool event( QEvent* e) override{
        if(e->type()== pipe0::streamEvent::type){
            auto eve = reinterpret_cast<pipe0::streamEvent*>(e);
            if (eve->getName() == pipe0::m_name){
                if (!m_init){
                    auto pip = pipe0::m_parent->find(m_act_name, false);
                    if (pip)
                        pipe<T, F>::m_func = dynamic_cast<pipe<T, F>*>(pip)->m_func;
                    m_init = true;
                }
                auto stm = std::dynamic_pointer_cast<stream<T>>(eve->getStream());
                QThreadPool::globalInstance()->start(new parallelTask<T, F>(this, stm));
            }
        }
        return true;
    }
private:
    bool m_init = false;
    QString m_act_name;
    friend parallelTask<T, F>;
    friend pipeline;
};

DSTDLL void connectPipelines(const QJsonArray& aPipelines);

template <typename T, template<class, typename> class P = pipe>
class regPip
{
public:
    regPip(pipeFunc<T> aFunc, const QJsonObject& aParam = QJsonObject(), const QString& aPrevious = ""){
        auto pip = pipeline::instance()->add<T, P>(aFunc, aParam);
        if (aPrevious != "")
            rea::pipeline::instance()->find(aPrevious)->next(pip->actName());
    }
};

#define extendTrigger(aType, aPipe, aPipeline) \
    rea::pipeline::instance()->add<aType>([](rea::stream<aType>* aInput){ \
        aInput->out(); \
    }, rea::Json("name", STR(aPipeline##_##aPipe), \
                 "befored", #aPipe, \
                 "external", #aPipeline))

#define extendListener(aType, aPipe, aPipeline) \
    rea::pipeline::instance()->find(#aPipe) \
    ->nextF<aType>([](rea::stream<aType>* aInput){ \
        aInput->out(); \
    }, "", rea::Json("name", STR(aPipeline##_##aPipe), \
                     "external", #aPipeline))

}

#endif

