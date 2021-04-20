#include "reaJS.h"
#include <QQmlApplicationEngine>

namespace rea {

pipelineJS::pipelineJS() : pipeline("js"){
    pipeline::instance()->supportType<pipelineJS*>([](stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<stream<pipelineJS*>*>(aInput)->data());
    });
    pipeline::instance()->add<double>([](stream<double>* aInput){
        auto pip_js = reinterpret_cast<pipelineJS*>(pipeline::instance("js"));
        aInput->outs<pipelineJS*>(pip_js)->scope()->cache<pipelineJS*>("pipeline", pip_js);
    }, rea::Json("name", "pipelineJSObject", "external", "qml"));
};

void pipelineJS::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFromOutside){
    if (!aStream->supportedType())
        throw "not supported type";
    executeJSPipe(aName,  aStream->QData(), aStream->tag(), aStream->scope()->toList(), aSync, aFromOutside);
}

void pipelineJS::tryExecuteOutsidePipe(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, const QString& aFlag){
    if (aData.type() == QVariant::Type::Map)
        tryExecutePipeOutside(aName, in(aData.toJsonObject(), aTag, std::make_shared<scopeCache>(aScope)), aSync, aFlag);
    else if (aData.type() == QVariant::Type::List)
        tryExecutePipeOutside(aName, in(aData.toJsonArray(), aTag, std::make_shared<scopeCache>(aScope)), aSync, aFlag);
    else if (aData.type() == QVariant::Type::String)
        tryExecutePipeOutside(aName, in(aData.toString(), aTag, std::make_shared<scopeCache>(aScope)), aSync, aFlag);
    else if (aData.type() == QVariant::Type::Bool)
        tryExecutePipeOutside(aName, in(aData.toBool(), aTag, std::make_shared<scopeCache>(aScope)), aSync, aFlag);
    else if (aData.type() == QVariant::Type::Double)
        tryExecutePipeOutside(aName, in(aData.toDouble(), aTag, std::make_shared<scopeCache>(aScope)), aSync, aFlag);
    else{
        std::cout << aData.type() << std::endl;
        assert(0);
    }
}

void pipelineJS::removePipeOutside(const QString& aName){
    pipeline::removePipeOutside(aName);
}

void pipelineJS::remove(const QString& aName, bool){
    removeJSPipe(aName);
}

static regPip<std::shared_ptr<pipeline*>> reg_create_jspipeline([](stream<std::shared_ptr<pipeline*>>* aInput){
    *aInput->data() = new pipelineJS();
}, rea::Json("name", "createjspipeline"));


static regPip<QQmlApplicationEngine*> reg_dynamic_dll([](stream<QQmlApplicationEngine*>* aInput){
    //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
    rea::pipeline::instance("js");
    aInput->out();
}, rea::Json("name", "install0_js"), "initRea");

}

