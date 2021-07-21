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
            m_data.insert(i, in(val.toObject()));
        else if (val.isArray())
            m_data.insert(i, in(val.toArray()));
        else if (val.isBool())
            m_data.insert(i, in(val.toBool()));
        else if (val.isString())
            m_data.insert(i, in(val.toString()));
        else if (val.isDouble())
            m_data.insert(i, in(val.toDouble()));
    }
}

scopeCache::~scopeCache(){

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

/*static QHash<QString, QQueue<std::shared_ptr<QEventLoop>>> async_busy;
std::shared_ptr<QEventLoop> stream0::waitLastAsync(const QString& aName){
    auto lops = tryFind(&async_busy, aName);
    auto lop = std::make_shared<QEventLoop>();
    lops->push_back(lop);
    if (lops->size() > 1){
        std::cout << aName.toStdString() << " locked2" << std::endl;
        lop->exec();
    }
    return lop;
}

void stream0::freeAsync(const QString& aName){
    auto lops = tryFind(&async_busy, aName);
    lops->pop_front();
    if (lops->size() && lops->front()->isRunning())
        lops->front()->exit();
}*/

pipe0::~pipe0(){
    pipe_counter--;
}

void pipe0::replaceTopo(pipe0* aOldPipe){
    m_next = aOldPipe->m_next;
    m_before = aOldPipe->m_before;
    m_around = aOldPipe->m_around;
    m_after = aOldPipe->m_after;
}

void pipe0::inPool(bool aReplace){
    auto old = m_parent->find(m_name, false);
    if (old){
        if (aReplace)
            replaceTopo(old);
        m_parent->remove(m_name);
    }
    m_parent->m_pipes.insert(m_name, this);
}

pipe0::pipe0(pipeline* aParent, const QString& aName, int aThreadNo){
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
}

void pipe0::resetTopo(){
    m_next.clear();
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

void pipe0::removeNext(const QString &aName, bool aAndDelete, bool aOutside){
    m_next.remove(aName);
    if (aAndDelete)
        m_parent->remove(aName, aOutside);
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

bool pipeline::externalNextGot(pipe0* aPipe, std::shared_ptr<stream0> aStream, const QString& aFrom){
    if (aPipe->m_external != this->name() && aPipe->m_external != aFrom){
        tryExecutePipeOutside(aPipe->actName(), aStream, QJsonObject(), aPipe->m_external);
        return false;
    }
    return true;
}

void pipeline::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    auto pip = find(aName, !aFutureNeed);
    if (!pip)
        return;
    if (aFrom != "" && !externalNextGot(pip, aStream, aFrom))
        return;
    if (!aSync.empty()){
        pip->resetTopo();
        auto nxts = aSync.value("next").toArray();
        for (auto i : nxts){
            auto nxt = i.toArray();
            pip->next(nxt[0].toString(), nxt[1].toString());
        }
        pip->setAspect(pip->m_before, aSync.value("before").toString());
        pip->setAspect(pip->m_after, aSync.value("after").toString());
        pip->setAspect(pip->m_around, aSync.value("around").toString());
    }
    pip->execute(aStream);
}

void pipeline::tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag){
    for (auto i : pipelines.keys())
        if (m_outside_pipelines.contains(i)){
            auto ln = pipelines.value(i);
            if (ln != this){
                if (aFlag == "any")
                    ln->execute(aName, aStream, aSync, true, name());
                else if (aFlag == ln->name())
                    ln->execute(aName, aStream, aSync);
            }
        }
}

void pipeFuture::removeNext(const QString& aName, bool aAndDelete, bool aOutside){
    for (auto i = m_next2.size() - 1; i >= 0; --i)
        if (m_next2[i].first == aName)
            m_next2.remove(i);
    if (aAndDelete)
        m_parent->remove(aName, aOutside);
}

pipeFuture::pipeFuture(pipeline* aParent, const QString& aName) : pipe0 (aParent){
    m_act_name = aName;

    if (m_parent->find(aName + "_pipe_add", false)){  //there will be another pipeFuture before, this future should inherit its records before it is removed
        auto pip = new pipeFuture0(m_parent, aName);
        pip->inPool(false);
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
            pip->inPool(false);
            call<int>(aName + "_pipe_add", 0);
            remove(aName + "_pipe_add", false);
            remove(aName, false);
        }
    }
    if (aOutside)
        removePipeOutside(aName);
}

void pipeline::updateOutsideRanges(const QSet<QString>& aRanges){
    for (auto i : aRanges)
        m_outside_pipelines.insert(i);
}

void pipeline::removePipeOutside(const QString& aName){
    for (auto i : pipelines.keys())
        if (m_outside_pipelines.contains(i)){
            auto ln = pipelines.value(i);
            if (ln != this)
                ln->remove(aName);
        }
}

QThread* pipeline::findThread(int aNo){
    auto thread = m_threads.find(aNo);
    if (thread == m_threads.end()){
        auto tmp = new QThread();
        tmp->start();
        m_threads.insert(aNo, tmp);
        thread = m_threads.find(aNo);
    }
    return thread.value();
}

pipeline* pipeline::instance(const QString& aName){
    if (!pipelines.contains(aName)){
        if (aName == getDefaultPipelineName())
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
    if (aName == getDefaultPipelineName()){
        QThreadPool::globalInstance()->setMaxThreadCount(8);
        supportType<QString>([](stream0* aInput){
            return QVariant::fromValue(reinterpret_cast<stream<QString>*>(aInput)->data());
        });
        supportType<QJsonObject>([](stream0* aInput){
            return QVariant::fromValue(reinterpret_cast<stream<QJsonObject>*>(aInput)->data());
        });
        supportType<QJsonArray>([](stream0* aInput){
            return QVariant::fromValue(reinterpret_cast<stream<QJsonArray>*>(aInput)->data());
        });
        supportType<double>([](stream0* aInput){
            return QVariant::fromValue(reinterpret_cast<stream<double>*>(aInput)->data());
        });
        supportType<bool>([](stream0* aInput){
            return QVariant::fromValue(reinterpret_cast<stream<bool>*>(aInput)->data());
        });

        add<double>([](rea::stream<double>* aInput){
            std::cout << getDefaultPipelineName().toStdString() + "_pipe_counter: " << pipe_counter << std::endl;
            std::cout << getDefaultPipelineName().toStdString() + "_stream_counter: " << stream_counter << std::endl;
            aInput->out();
        }, rea::Json("name", "reportCLeak", "external", "js"));
    }
}

pipeline::~pipeline(){
    for (auto i : m_threads)
        if (i->isRunning()){
            i->terminate();
            i->wait();
        }
    for (auto i : m_pipes.values())
        delete i;
    m_pipes.clear();
}

}
