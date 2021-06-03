#include "reaQML.h"
#include <QFile>
#include <QWindow>
#include <QJsonDocument>
#include <QGuiApplication>
#include <QDoubleValidator>
#include <QQmlApplicationEngine>

namespace rea {

static QQmlApplicationEngine* qml_engine = nullptr;

QJSValue qmlstream2JSValue(qmlStream* aStream){
   return qml_engine->toScriptValue(aStream);
}

template <typename T>
class valType{
public:
    static T data(const QJSValue&){
        return T();
    }
};

template <>
class valType<QJsonObject>{
public:
    static QJsonObject data(const QJSValue& aValue){
        return QJsonObject::fromVariantMap(aValue.toVariant().toMap());
    }
};

template <>
class valType<QJsonArray>{
public:
    static QJsonArray data(const QJSValue& aValue){
        return QJsonArray::fromVariantList(aValue.toVariant().toList());
    }
};

template <>
class valType<QString>{
public:
    static QString data(const QJSValue& aValue){
        return aValue.toString();
    }
};

template <>
class valType<double>{
public:
    static double data(const QJSValue& aValue){
        return aValue.toNumber();
    }
};

template <>
class valType<bool>{
public:
    static bool data(const QJSValue& aValue){
        return aValue.toBool();
    }
};

static QHash<QString, pipeline*> pipelines;

pipeline* pipelineQML::qmlinstance(const QString& aName){
    if (!pipelines.contains(aName)){
        if (aName == "c++" || aName == "qml")
            pipelines.insert(aName, pipeline::instance());
        else{
            auto pl = std::make_shared<pipeline*>();
            instance()->call<std::shared_ptr<pipeline*>>("createqml" + aName + "pipeline", pl);
            if (*pl)
                pipelines.insert(aName, *pl);
        }
    }
    return pipelines.value(aName);
}

pipelineQML::pipelineQML() : pipeline("qml"){
    pipeline::instance()->supportType<QVariant>([](stream0* aInput){
        return reinterpret_cast<stream<QVariant>*>(aInput)->data();
    });  //qmlEngine can only be used on main thread, so use qvariant instead of qjsvalue
}

void pipelineQML::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    if (!aStream->supportedType())
        throw "not supported type";
    pipeline::execute(aName, in(aStream->QData(), aStream->tag(), aStream->scope()), aSync, aFutureNeed, aFrom);
}

pipelineQMLJS::pipelineQMLJS() : pipeline("js"){
    pipeline::instance()->supportType<pipelineQML*>([](stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<stream<pipelineQML*>*>(aInput)->data());
    });
    pipeline::instance()->add<double>([](stream<double>* aInput){
        auto pip_qml = reinterpret_cast<pipelineQML*>(pipeline::instance("qmljs"));
        aInput->outs<pipelineQML*>(pip_qml)->scope()->cache<pipelineQML*>("pipeline", pip_qml);
    }, rea::Json("name", "pipelineQMLObject", "external", "qml"));
}

void pipelineQMLJS::executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, const QString& aFlag){
    if (aFlag == "any")
        pipeline::instance("qml")->execute(aName, in(aData, aTag, std::make_shared<scopeCache>(aScope)), aSync, true, name());
    else if (aFlag == "qml")
        pipeline::instance("qml")->execute(aName, in(aData, aTag, std::make_shared<scopeCache>(aScope)), aSync);
}

void pipelineQMLJS::removeFromJS(const QString& aName){
    pipeline::instance("qml")->remove(aName, false);
}

void pipelineQMLJS::remove(const QString& aName, bool){
    removeJSPipe(aName);
}

void pipelineQMLJS::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    if (!aStream->supportedType())
        throw "not supported type";
    executeJSPipe(aName,  aStream->QData(), aStream->tag(), aStream->scope()->toList(), aSync, aFutureNeed, aFrom);
}

static regPip<std::shared_ptr<pipeline*>> reg_create_qmljspipeline([](stream<std::shared_ptr<pipeline*>>* aInput){
    *aInput->data() = new pipelineQMLJS();
}, rea::Json("name", "createqmljspipeline"));

static QSet<QString> qml_ranges = {"c++", "qml", "qmljs"};

void pipelineQML::removePipeOutside(const QString& aName, QSet<QString>*){
    pipeline::removePipeOutside(aName, &qml_ranges);
}

void pipelineQML::tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag, QSet<QString>*) {
    auto aData = aStream->QData();
    if (aData.type() == QVariant::Type::Map || aData.type() == QMetaType::QJsonObject)
        pipeline::tryExecutePipeOutside(aName, in(aData.toJsonObject(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else if (aData.type() == QVariant::Type::List || aData.type() == QMetaType::QJsonArray)
        pipeline::tryExecutePipeOutside(aName, in(aData.toJsonArray(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else if (aData.type() == QVariant::Type::String)
        pipeline::tryExecutePipeOutside(aName, in(aData.toString(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else if (aData.type() == QVariant::Type::Bool)
        pipeline::tryExecutePipeOutside(aName, in(aData.toBool(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else if (aData.type() == QVariant::Type::Double)
        pipeline::tryExecutePipeOutside(aName, in(aData.toDouble(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else if (aData.type() == QVariant::Type::Int)
        pipeline::tryExecutePipeOutside(aName, in<double>(aData.toInt(), aStream->tag(), aStream->scope()), aSync, aFlag, &qml_ranges);
    else{
        std::cout << aData.type() << std::endl;
        throw "not supported type";
    }
}

bool pipelineQML::externalNextGot(pipe0* aPipe, std::shared_ptr<stream0> aStream, const QString& aFrom, QSet<QString>*){
    return pipeline::externalNextGot(aPipe, aStream, aFrom, &qml_ranges);
}

static regPip<std::shared_ptr<pipeline*>> reg_create_qmlpipeline([](stream<std::shared_ptr<pipeline*>>* aInput){
    *aInput->data() = new pipelineQML();
}, rea::Json("name", "createqmlpipeline"));

qmlScopeCache::qmlScopeCache(std::shared_ptr<scopeCache> aScope){
    m_scope = aScope;
}

qmlScopeCache::~qmlScopeCache(){

}

QJSValue qmlScopeCache::cache(const QString& aName, QJSValue aData){
    if (aData.isArray())
        m_scope->cache<QJsonArray>(aName, valType<QJsonArray>::data(aData));
    else if (aData.isObject())
        m_scope->cache<QJsonObject>(aName, valType<QJsonObject>::data(aData));
    else if (aData.isNumber())
        m_scope->cache<double>(aName, valType<double>::data(aData));
    else if (aData.isBool())
        m_scope->cache<bool>(aName, valType<bool>::data(aData));
    else if (aData.isString())
        m_scope->cache<QString>(aName, valType<QString>::data(aData));
    else
        qFatal("Invalid data type in qmlScopeCache");
    return qml_engine->toScriptValue(this);
}

QJSValue qmlScopeCache::data(const QString& aName){
    auto stm = m_scope->dataStream(aName);
    if (stm)
        return qml_engine->toScriptValue(stm->QData());
    else
        return QJSValue::NullValue;
}

static int stream_counter = 0;
static int pipe_counter = 0;

qmlStream::qmlStream(){
    stream_counter++;
}

qmlStream::qmlStream(std::shared_ptr<stream<QVariant>> aStream){
    stream_counter++;

    m_stream = aStream;
}

qmlStream::~qmlStream(){
    --stream_counter;
}

QJSValue qmlStream::data(){
    return qml_engine->toScriptValue(m_stream->data());
}
QString qmlStream::tag(){
    return m_stream->tag();
}

QJSValue qmlStream::scope(bool aNew){
    //https://stackoverflow.com/questions/59267782/how-to-safely-use-qqmlenginecppownership
    auto ret = new qmlScopeCache(m_stream->scope(aNew));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlStream::setData(QJSValue aData){
    m_stream->setData(aData.toVariant());
    return qml_engine->toScriptValue(this);
}

QJSValue qmlStream::out(const QString& aTag){
    m_stream->out(aTag);
    return qml_engine->toScriptValue(this);
}

QJSValue qmlStream::outs(QJSValue aOut, const QString& aNext, const QString& aTag){
    auto stm = m_stream->outs(aOut.toVariant(), aNext, aTag);
    auto ret = new qmlStream(std::dynamic_pointer_cast<stream<QVariant>>(stm->shared_from_this()));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlStream::outsB(QJSValue aOut, const QString& aNext, const QString& aTag){
    outs(aOut, aNext, aTag);
    return qml_engine->toScriptValue(this);
}

void qmlStream::noOut(){
    m_stream->noOut();
}

QJSValue qmlStream::asyncCall(const QString& aName, bool aEventLevel){
    auto ret = new qmlStream(m_stream->asyncCall<QVariant>(aName, aEventLevel, pipeline::instance("qml")));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlStream::asyncCallF(QJSValue aFunc, const QJsonObject& aParam, bool aEventLevel){
    auto ret = new qmlStream(m_stream->asyncCallF<QVariant, pipe, QJSValue, QJSValue>(aFunc, aParam, aEventLevel, pipeline::instance("qml")));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

qmlPipe::qmlPipe(pipeline* aParent, const QString& aName){
    ++pipe_counter;

    m_parent = aParent;
    m_name = aName;
}

qmlPipe::~qmlPipe(){
    --pipe_counter;
}

QJSValue qmlPipe::next(const QString& aName, const QString& aTag){
    qmlPipe* ret = new qmlPipe(m_parent, aName);
    m_parent->find(m_name)->next(aName, aTag);
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlPipe::nextB(const QString& aName, const QString& aTag){
    next(aName, aTag);
    return qml_engine->toScriptValue(this);
}

QString doAdd(QJSValue aFunc, const QJsonObject& aParam){
    return pipeline::instance()
            ->call<QString>("createQMLPipe" + aParam.value("type").toString(),
                            "",
                            std::make_shared<scopeCache>()->cache("func", aFunc)->cache("param", aParam))
            ->data();
}

QJSValue qmlPipe::nextF(QJSValue aFunc, const QString& aTag, const QJsonObject& aParam){
    return next(doAdd(aFunc, aParam), aTag);
}

QJSValue qmlPipe::nextFB(QJSValue aFunc, const QString& aTag, const QJsonObject& aParam){
    nextF(aFunc, aTag, aParam);
    return qml_engine->toScriptValue(this);
}

void qmlPipe::removeNext(const QString& aName){
    m_parent->find(m_name)->removeNext(aName);
}

void qmlPipe::removeAspect(const QString& aType, const QString& aAspect){
    auto pip = m_parent->find(m_name);
    if (aType == "before")
        pip->removeAspect(pipe0::AspectBefore, aAspect);
    else if (aType == "after")
        pip->removeAspect(pipe0::AspectAfter, aAspect);
    else if (aType == "around")
        pip->removeAspect(pipe0::AspectAround, aAspect);
    else
        throw "no this kind of aspect";
}

QJSValue qmlPipeline::run(const QString& aName, const QJSValue& aInput, const QString& aTag, const QJSValue& aScope){
    std::shared_ptr<scopeCache> sp_data = nullptr;
    if (aScope.isQObject()){
        auto sp = reinterpret_cast<qmlScopeCache*>(aScope.toQObject());
        sp_data = sp->m_scope;
    }else if (aScope.isObject())
        sp_data = std::make_shared<scopeCache>(valType<QJsonObject>::data(aScope));
    auto ret = new qmlStream(pipeline::instance("qml")->run(aName, aInput.toVariant(), aTag, sp_data));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlPipeline::input(const QJSValue& aInput, const QString& aTag, const QJsonObject& aScopeCache, bool aAutoTag){
    auto ret = new qmlStream(in(aInput.toVariant(), aTag, std::make_shared<scopeCache>(aScopeCache), aAutoTag));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlPipeline::call(const QString& aName, const QJSValue& aInput, const QJsonObject& aScope){
    auto stm = pipeline::instance("qml")->call(aName, aInput.toVariant(), std::make_shared<scopeCache>(aScope));
    auto ret = new qmlStream(std::dynamic_pointer_cast<stream<QVariant>>(stm));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

void qmlPipeline::remove(const QString& aName, bool aOutside){
    pipeline::instance("qml")->remove(aName, aOutside);
}

QObject* qmlPipeline::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new qmlPipeline();
}

QJSValue qmlPipeline::add(QJSValue aFunc, const QJsonObject& aParam){
    auto ret = new qmlPipe(pipeline::instance("qml"), doAdd(aFunc, aParam));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlPipeline::find(const QString& aName){
    qmlPipe* ret = new qmlPipe(pipeline::instance("qml"), aName);
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlPipeline::asyncCall(const QString& aName, const QJSValue& aInput, bool aEventLevel){
    auto ret = new qmlStream(pipeline::instance("qml")->asyncCall(aName, aInput.toVariant(), aEventLevel));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

static bool m_language_updated;
static QJsonObject translates;

qmlPipeline::qmlPipeline(){
    m_language_updated = false;
    QFile fl(".language");
    if (fl.open(QFile::ReadOnly)){
        translates = QJsonDocument::fromJson(fl.readAll()).object();
        fl.close();
    }

    pipeline::instance()->add<double>([](rea::stream<double>* aInput){
        std::cout << "qml_pipe_counter: " << pipe_counter << std::endl;
        std::cout << "qml_stream_counter: " << stream_counter << std::endl;
        aInput->out();
    }, rea::Json("name", "reportQMLLeak", "external", "js"));
}

qmlPipeline::~qmlPipeline(){
    if (m_language_updated){
        QFile fl(".language");
        if (fl.open(QFile::WriteOnly)){
            fl.write(QJsonDocument(translates).toJson());
            fl.close();
        }
    }
}

//https://stackoverflow.com/questions/35178569/doublevalidator-is-not-checking-the-ranges-properly
class TextFieldDoubleValidator : public QDoubleValidator {
public:
    TextFieldDoubleValidator (QObject * parent = nullptr) : QDoubleValidator(parent) {}
    TextFieldDoubleValidator (double bottom, double top, int decimals, QObject * parent) :
                                                                                         QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString & s, int&) const {
        if (s.isEmpty() || (s.startsWith("-") && s.length() == 1)) {
            // allow empty field or standalone minus sign
            return QValidator::Intermediate;
        }
        // check length of decimal places
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1) {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};

static regPip<QQmlApplicationEngine*> reg_recative2_qml([](stream<QQmlApplicationEngine*>* aInput){
    auto cfg = aInput->scope()->data<QJsonObject>("rea-qml");
    //qInstallMessageHandler
    if (cfg.value("use").toBool(true)){
        qml_engine = aInput->data();
        //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
        qmlRegisterSingletonType<qmlPipeline>("Pipeline", 1, 0, "Pipeline", &qmlPipeline::qmlInstance);
        qmlRegisterType<TextFieldDoubleValidator>("TextFieldDoubleValidator", 1, 0, "TextFieldDoubleValidator");
    }
    aInput->out();
}, rea::Json("name", "install2_qml"), "initRea");

QString qmlPipeline::tr(const QString& aOrigin){
    return tr0(aOrigin);
}

QString tr0(const QString& aOrigin){
    auto key = aOrigin.trimmed();
    if (!translates.contains(key)){
        translates.insert(key, aOrigin);
        m_language_updated = true;
    }
    return translates.value(key).toString(aOrigin);
}

regQMLPipe(Partial)
regQMLPipe(Delegate)
regQMLPipe()

static regPip<QQmlApplicationEngine*> reg_qmljs_linker([](stream<QQmlApplicationEngine*>* aInput){
    //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
    rea::pipeline::instance("qmljs");
    aInput->out();
}, rea::Json("name", "install0_qmljs"), "initRea");

}
