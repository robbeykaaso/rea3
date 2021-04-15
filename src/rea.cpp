#include "rea.h"
#include <mutex>
#include <sstream>
#include <QFile>
#include <QJsonDocument>
#include <QQueue>
#include "reaJS.h"

namespace rea {

scopeCache::scopeCache(const QJsonObject& aData){
    for (auto i : aData.keys()){
        auto val = aData.value(i);
        if (val.isObject())
            m_data.insert(i, std::make_shared<stream<QJsonObject>>(val.toObject()));
        else if (val.isArray())
            m_data.insert(i, std::make_shared<stream<QJsonArray>>(val.toArray()));
        else if (val.isBool())
            m_data.insert(i, std::make_shared<stream<bool>>(val.toBool()));
        else if (val.isString())
            m_data.insert(i, std::make_shared<stream<QString>>(val.toString()));
        else if (val.isDouble())
            m_data.insert(i, std::make_shared<stream<double>>(val.toDouble()));
    }
}

QVariantList scopeCache::toList(){
    QVariantList ret;
    for (auto i : m_data.keys()){
        ret.push_back(i);
        ret.push_back(m_data.value(i)->QData());
    }
    return ret;
}

static int pipe_counter = 0;
static int stream_counter = 0;

stream0::stream0(const QString& aTag) {
    stream_counter++;

    m_tag = aTag;
    m_scope = nullptr;
}

stream0::~stream0(){
    stream_counter--;
}

static QHash<QThread*, QQueue<std::shared_ptr<QEventLoop>>> async_busy;
std::shared_ptr<QEventLoop> stream0::waitLastAsync(const QString& aName){
    auto lops = tryFind(&async_busy, QThread::currentThread());
    auto lop = std::make_shared<QEventLoop>();
    lops->push_back(lop);
    if (lops->size() > 1){
        std::cout << aName.toStdString() << " locked2" << std::endl;
        lop->exec();
    }
    return lop;
}

void stream0::freeAsync(){
    auto lops = tryFind(&async_busy, QThread::currentThread());
    lops->pop_front();
    if (lops->size() && lops->front()->isRunning())
        lops->front()->exit();
}

pipe0::~pipe0(){
    pipe_counter--;
}

pipe0::pipe0(pipeline* aParent, const QString& aName, int aThreadNo, bool aReplace){
    pipe_counter++;
    m_parent = aParent;
    m_external = aParent->name();
    if (aName == "")
        m_name = rea::generateUUID();
    else
        m_name = aName;
    if (aThreadNo != 0){
        m_thread = m_parent->findThread(aThreadNo);
        moveToThread(m_thread);
    }
    auto old = m_parent->find(m_name, false);
    if (old){
        if (aReplace){
            m_next = old->m_next;
            m_before = old->m_before;
            m_around = old->m_around;
            m_after = old->m_after;
        }
        m_parent->remove(m_name);
    }
    m_parent->m_pipes.insert(m_name, this);
}

void pipe0::resetTopo(){
    m_next.clear();
    m_before = "";
    m_around = "";
    m_after = "";
}

pipe0* pipe0::nextP(pipe0* aNext, const QString& aTag){
    auto tags = aTag.split(";");
    for (auto i : tags)
        insertNext(aNext->actName(), i);
    return aNext;
}

pipe0* pipe0::next(const QString& aName, const QString& aTag){
    auto tags = aTag.split(";");
    for (auto i : tags)
        insertNext(aName, i);
    auto nxt = m_parent->find(aName);
    return nxt;
}

void pipe0::removeNext(const QString &aName){
    m_next.remove(aName);
}

void pipe0::removeAspect(pipe0::AspectType aType, const QString& aAspect){
    QString* tar = nullptr;
    if (aType == pipe0::AspectType::AspectAfter)
        tar = &m_after;
    else if (aType == pipe0::AspectType::AspectBefore)
        tar = &m_before;
    else if (aType == pipe0::AspectType::AspectAround)
        tar = &m_around;
    if (aAspect == "")
        *tar = "";
    else{
        auto idx = tar->indexOf(aAspect);
        if (idx > 0)
            *tar = tar->remove(idx - 1, aAspect.length());
        else if (!idx)
            *tar = tar->remove(idx, aAspect.length());
    }
}

pipe0* pipe0::nextPB(pipe0* aNext, const QString& aTag){
    nextP(aNext, aTag);
    return this;
}

pipe0* pipe0::nextB(const QString& aName, const QString& aTag){
    next(aName, aTag);
    return this;
}

void pipe0::tryExecutePipe(const QString& aName, std::shared_ptr<stream0> aStream){
    auto pip = m_parent->find(aName);
    if (pip->m_external != m_parent->name())
        m_parent->tryExecutePipeOutside(pip->actName(), aStream, QJsonObject(), pip->m_external);
    else
        pip->execute(aStream);
}

void pipe0::doNextEvent(const QMap<QString, QString>& aNexts, std::shared_ptr<stream0> aStream){
    auto outs = aStream->m_outs;
    aStream->m_outs = nullptr;
    if (outs){
        if (outs->size() > 0){
            for (auto i : *outs)
                if (i.first == ""){
                    for (auto j : aNexts.keys())
                        tryExecutePipe(j, i.second);
                }else
                    tryExecutePipe(i.first, i.second);
        }else
            for (auto i : aNexts.keys())
                tryExecutePipe(i, aStream);
    }
}

void pipe0::setAspect(QString& aTarget, const QString& aAspect){
    if (aTarget.indexOf(aAspect) < 0){
        if (aTarget != "")
            aTarget += ";";
        aTarget += aAspect;
    }
}

void pipe0::execute(std::shared_ptr<stream0> aStream){
    if (QThread::currentThread() == m_thread){
        streamEvent nxt_eve(m_name, aStream);
        QCoreApplication::sendEvent(this, &nxt_eve);
    }else{
        auto nxt_eve = std::make_unique<streamEvent>(m_name, aStream);
        QCoreApplication::postEvent(this, nxt_eve.release());  //https://stackoverflow.com/questions/32583078/does-postevent-free-the-event-after-posting-double-free-or-corruption // still memory leak, reason is unknown
    }
}

void pipeFuture::resetTopo(){
    m_next2.clear();
}

bool pipeFuture::event( QEvent* e) {
    if(e->type()== streamEvent::type){
        auto eve = reinterpret_cast<streamEvent*>(e);
        if (eve->getName() == m_name){
            QJsonObject sync;
            QJsonArray nxts;
            for (auto i : m_next2)
                nxts.push_back(rea::JArray(i.first, i.second));
            if (nxts.size() > 0)
                sync.insert("next", nxts);
            if (m_before != "")
                sync.insert("before", m_before);
            if (m_around != "")
                sync.insert("around", m_around);
            if (m_after != "")
                sync.insert("after", m_after);
            m_parent->tryExecutePipeOutside(actName(), eve->getStream(), sync, "any");
        }
    }
    return true;
}

void pipeFuture::execute(std::shared_ptr<stream0> aStream){
    pipe0::execute(aStream);
}

void pipeFuture::insertNext(const QString& aName, const QString& aTag){
    m_next2.push_back(QPair<QString, QString>(aName, aTag));
}

class pipeFuture0 : public pipe0 {  //the next of pipePartial may be the same name but not the same previous
public:
    pipeFuture0(pipeline* aParent, const QString& aName) : pipe0(aParent, aName){
    }
protected:
    void insertNext(const QString& aName, const QString& aTag) override{
        m_next2.push_back(QPair<QString, QString>(aName, aTag));
    }
private:
    QVector<QPair<QString, QString>> m_next2;
    friend pipeFuture;
};

static QHash<QString, pipeline*> pipelines;

void pipeline::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFromOutside){
    auto pip = find(aName, !aFromOutside);
    if (!pip)
        return;
    if (!aSync.empty()){
        pip->resetTopo();
        auto nxts = aSync.value("next").toArray();
        for (auto i : nxts){
            auto nxt = i.toArray();
            pip->insertNext(nxt[0].toString(), nxt[1].toString());
        }
        pip->setAspect(pip->m_before, aSync.value("before").toString());
        pip->setAspect(pip->m_after, aSync.value("after").toString());
        pip->setAspect(pip->m_around, aSync.value("around").toString());
    }
    pip->execute(aStream);
}

void pipeline::tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag){
    for (auto i : pipelines)
        if (i != this){
            if (aFlag == "any")
                i->execute(aName, aStream, aSync, true);
            else if (aFlag == i->name())
                i->execute(aName, aStream, aSync);
        }
}

void pipeFuture::removeNext(const QString& aName){
    for (auto i = m_next2.size() - 1; i >= 0; --i)
        if (m_next2[i].first == aName)
            m_next2.remove(i);
}

pipeFuture::pipeFuture(pipeline* aParent, const QString& aName) : pipe0 (aParent){
    m_act_name = aName;

    if (m_parent->find(aName + "_pipe_add", false)){  //there will be another pipeFuture before, this future should inherit its records before it is removed
        auto pip = new pipeFuture0(m_parent, aName);
        m_parent->call<int>(aName + "_pipe_add", 0);
        for (auto i : pip->m_next2)
            insertNext(i.first, i.second);
        m_external = pip->m_external;
        setAspect(m_before, pip->m_before);
        setAspect(m_around, pip->m_around);
        setAspect(m_after, pip->m_after);
        m_parent->doRemove(aName);
    }
    m_parent->add<int>([this, aName](const stream<int>*){
        auto this_event = m_parent->find(aName, false);
        for (auto i : m_next2)
            this_event->insertNext(i.first, i.second);
        this_event->m_external = m_external;
        setAspect(this_event->m_before, m_before);
        setAspect(this_event->m_around, m_around);
        setAspect(this_event->m_after, m_after);
        m_parent->doRemove(m_name);
    }, rea::Json("name", aName + "_pipe_add"));
}

void pipeline::doRemove(const QString& aName){
    auto pip = m_pipes.value(aName);
    m_pipes.remove(aName);
    delete pip;
}

void pipeline::remove(const QString& aName, bool aOutside){
    auto pip = m_pipes.value(aName);
    if (pip){
        m_pipes.remove(aName);
        delete pip;
    }else {
        pip = find(aName + "_pipe_add", false);
        if (pip){
            pip = new pipeFuture0(this, aName);
            call<int>(aName + "_pipe_add", 0);
            remove(aName + "_pipe_add", false);
            remove(aName, false);
        }
    }
    if (aOutside)
        removePipeOutside(aName);
}

void pipeline::removePipeOutside(const QString& aName){
    for (auto i : pipelines.values())
        if (i != this)
            i->remove(aName);
}

QThread* pipeline::findThread(int aNo){
    auto thread = m_threads.find(aNo);
    if (thread == m_threads.end()){
        auto tmp = std::make_shared<QThread>();
        tmp->start();
        m_threads.insert(aNo, tmp);
        thread = m_threads.find(aNo);
    }
    return thread->get();
}

pipeline* pipeline::instance(const QString& aName){
    if (!pipelines.contains(aName)){
        if (aName == "")
            pipelines.insert(aName, new pipeline());
        else{
            auto pl = std::make_shared<pipeline*>();
            instance()->call<std::shared_ptr<pipeline*>>("create" + aName + "pipeline", pl);
            if (*pl)
                pipelines.insert(aName, *pl);
        }

    }
    return pipelines.value(aName);
}

pipeline::pipeline(const QString& aName){
    m_name = aName;
    if (aName == ""){
        QThreadPool::globalInstance()->setMaxThreadCount(8);
        supportType<QString>();
        supportType<QJsonObject>();
        supportType<QJsonArray>();
        supportType<double>();
        supportType<bool>();

        add<double>([](rea::stream<double>* aInput){
            std::cout << "c++_pipe_counter: " << pipe_counter << std::endl;
            std::cout << "c++_stream_counter: " << stream_counter << std::endl;
            aInput->out();
        }, rea::Json("name", "reportCLeak", "external", true));
    }
}

pipeline::~pipeline(){
    for (auto i : m_threads)
        if (i.get()->isRunning()){
            i.get()->terminate();
            i->wait();
        }
    for (auto i : m_pipes.values())
        delete i;
    m_pipes.clear();
}

}
