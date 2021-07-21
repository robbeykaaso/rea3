#include "reaJS.h"
#include <QQmlApplicationEngine>

namespace rea {

pipelineJS::pipelineJS(const QString& aName) : pipeline(aName){
    pipeline::instance()->supportType<pipelineJS*>([](stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<stream<pipelineJS*>*>(aInput)->data());
    });
    pipeline::instance()->add<double>([this](stream<double>* aInput){
        auto pip_js = reinterpret_cast<pipelineJS*>(pipeline::instance(name()));
        aInput->outs<pipelineJS*>(pip_js)->scope()->cache<pipelineJS*>("pipeline", pip_js);
    }, rea::Json("name", "pipeline" + name().toUpper() + "Object", "external", getDefaultQMLPipelineName()));
};

void pipelineJS::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    if (!aStream->supportedType())
        throw "not supported type";
    executeJSPipe(aName,  aStream->QData(), aStream->tag(), aStream->scope()->toList(), aSync, aFutureNeed, aFrom);
}

void pipelineJS::remove(const QString& aName, bool){
    removeJSPipe(aName);
}

std::shared_ptr<stream0> makeInput(const QVariant& aData, const QString& aTag, const QJsonObject& aScope){
    std::shared_ptr<stream0> ret = nullptr;
    QJsonObject scp;
    for (auto i : aScope.keys())
        scp.insert(i, aScope.value(i));
    if (aData.type() == QVariant::Type::Map)
        ret = in(aData.toJsonObject(), aTag, std::make_shared<scopeCache>(scp));
    else if (aData.type() == QVariant::Type::List)
        ret = in(aData.toJsonArray(), aTag, std::make_shared<scopeCache>(scp));
    else if (aData.type() == QVariant::Type::String)
        ret = in(aData.toString(), aTag, std::make_shared<scopeCache>(scp));
    else if (aData.type() == QVariant::Type::Bool)
        ret = in(aData.toBool(), aTag, std::make_shared<scopeCache>(scp));
    else if (aData.type() == QVariant::Type::Double)
        ret = in(aData.toDouble(), aTag, std::make_shared<scopeCache>(scp));
    else{
        std::cout << aData.type() << std::endl;
        assert(0);
    }
    return ret;
}

void pipelineJS::executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag){
    rea::pipeline::instance()->execute(aName, makeInput(aData, aTag, aScope), aSync, aNeedFuture, aFlag);
}

void pipelineJS::removeFromJS(const QString& aName){
    rea::pipeline::instance()->remove(aName, false);
}

void environmentJS::setEnv(const QJsonObject& aEnvs){
    m_envs = aEnvs;
}

QJsonObject environmentJS::getEnv(){
    return m_envs;
}

static regPip<std::shared_ptr<pipeline*>> reg_create_jspipeline([](stream<std::shared_ptr<pipeline*>>* aInput){
    *aInput->data() = new pipelineJS();
}, rea::Json("name", "createjspipeline"));

}

