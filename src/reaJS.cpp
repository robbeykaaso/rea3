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
    }, rea::Json("name", "pipeline" + name().toUpper() + "Object", "external", "qml"));
};

void pipelineJS::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    if (!aStream->supportedType())
        throw "not supported type";
    executeJSPipe(aName,  aStream->QData(), aStream->tag(), aStream->scope()->toList(), aSync, aFutureNeed, aFrom);
}

void pipelineJS::remove(const QString& aName, bool){
    removeJSPipe(aName);
}

void pipelineJS::executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, const QString& aFlag){
    auto stm = [&](){
        std::shared_ptr<stream0> ret = nullptr;
        if (aData.type() == QVariant::Type::Map)
            ret = in(aData.toJsonObject(), aTag, std::make_shared<scopeCache>(aScope));
        else if (aData.type() == QVariant::Type::List)
            ret = in(aData.toJsonArray(), aTag, std::make_shared<scopeCache>(aScope));
        else if (aData.type() == QVariant::Type::String)
            ret = in(aData.toString(), aTag, std::make_shared<scopeCache>(aScope));
        else if (aData.type() == QVariant::Type::Bool)
            ret = in(aData.toBool(), aTag, std::make_shared<scopeCache>(aScope));
        else if (aData.type() == QVariant::Type::Double)
            ret = in(aData.toDouble(), aTag, std::make_shared<scopeCache>(aScope));
        else{
            std::cout << aData.type() << std::endl;
            assert(0);
        }
        return ret;
    };
    if (aFlag == "any")
        rea::pipeline::instance()->execute(aName, stm(), aSync, true, name());
    else if (aFlag == "c++")
        rea::pipeline::instance()->execute(aName, stm(), aSync);
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


static regPip<QQmlApplicationEngine*> reg_js_linker([](stream<QQmlApplicationEngine*>* aInput){
    qmlRegisterType<environmentJS>("EnvJS", 1, 0, "EnvJS");
    //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
    rea::pipeline::instance("js");
    aInput->out();
}, rea::Json("name", "install0_js"), "initRea");

}

