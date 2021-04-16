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

template <>
class typeTrait<QVariant> : public typeTrait0{
public:
    QString name() override{
        return "qvar";
    }
    QVariant QData(stream0* aStream) override{
        return reinterpret_cast<stream<QVariant>*>(aStream)->data();
        //return QVariant::fromValue<QJSValue>(reinterpret_cast<stream<QJSValue>*>(aStream)->data());
    }
};

pipelineQML::pipelineQML() : pipeline("qml"){
    pipeline::instance()->supportType<QVariant>();  //qmlEngine can only be used on main thread, so use qvariant instead of qjsvalue
}

void pipelineQML::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFromOutside){
    if (aStream->dataType() == "")
        throw "not supported type";
    pipeline::execute(aName, in(aStream->QData(), aStream->tag(), aStream->scope()), aSync, aFromOutside);
}

void pipelineQML::tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag) {
    auto aData = aStream->QData();
    if (aData.type() == QVariant::Type::Map)
        pipeline::tryExecutePipeOutside(aName, in(aData.toJsonObject(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else if (aData.type() == QVariant::Type::List)
        pipeline::tryExecutePipeOutside(aName, in(aData.toJsonArray(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else if (aData.type() == QVariant::Type::String)
        pipeline::tryExecutePipeOutside(aName, in(aData.toString(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else if (aData.type() == QVariant::Type::Bool)
        pipeline::tryExecutePipeOutside(aName, in(aData.toBool(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else if (aData.type() == QVariant::Type::Double)
        pipeline::tryExecutePipeOutside(aName, in(aData.toDouble(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else if (aData.type() == QVariant::Type::Int)
        pipeline::tryExecutePipeOutside(aName, in<double>(aData.toInt(), aStream->tag(), aStream->scope()), aSync, aFlag);
    else{
        throw "not supported type";
    }
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
    m_stream->outs(aOut.toVariant(), aNext, aTag);
    return qml_engine->toScriptValue(this);
}

QJSValue qmlStream::asyncCall(const QString& aName, const QString& aPipeline){
    auto ret = new qmlStream(m_stream->asyncCall<QVariant>(aName, pipeline::instance(aPipeline)));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

QJSValue qmlStream::asyncCallF(QJSValue aFunc, const QJsonObject& aParam, const QString& aPipeline){
    auto ret = new qmlStream(m_stream->asyncCallF<QVariant, pipe, QJSValue, QJSValue>(aFunc, aParam, pipeline::instance(aPipeline)));
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

void qmlPipe::resetTopo(){
    m_parent->find(m_name)->resetTopo();
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

void qmlPipeline::run(const QString& aName, const QJSValue& aInput, const QString& aTag, const QJsonObject& aScopeCache){
    pipeline::instance("qml")->run(aName, aInput.toVariant(), aTag, std::make_shared<scopeCache>(aScopeCache));
}

QJSValue qmlPipeline::input(const QJSValue& aInput, const QString& aTag, const QJsonObject& aScopeCache){
    auto ret = new qmlStream(in(aInput.toVariant(), aTag, std::make_shared<scopeCache>(aScopeCache)));
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::JavaScriptOwnership);
    return qml_engine->toScriptValue(ret);
}

void qmlPipeline::call(const QString& aName, const QJSValue& aInput){
    pipeline::instance("qml")->call(aName, aInput.toVariant());
}

void qmlPipeline::remove(const QString& aName){
    pipeline::instance("qml")->remove(aName);
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

QJSValue qmlPipeline::asyncCall(const QString& aName, const QJSValue& aInput){
    auto ret = new qmlStream(pipeline::instance("qml")->asyncCall(aName, aInput.toVariant()));
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
    }, rea::Json("name", "reportQMLLeak", "external", true));
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
    if (cfg.value("use").toBool(true)){
        QVector<QObject*> objs;
        qml_engine = aInput->data();
        //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
        qmlRegisterSingletonType<qmlPipeline>("Pipeline", 1, 0, "Pipeline", &qmlPipeline::qmlInstance);
        qmlRegisterType<TextFieldDoubleValidator>("TextFieldDoubleValidator", 1, 0, "TextFieldDoubleValidator");
        //engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath()); //https://recalll.co/ask/v/topic/qt-QML%3A-how-to-specify-image-file-path-relative-to-application-folder/55928bae7d3563c7088b7db1

        qml_engine->connect(qml_engine, &QQmlApplicationEngine::objectCreated, [&objs](QObject *object, const QUrl){
            if (object)
                objs.push_back(object);
        });
        auto prm = cfg.value("param").toObject();
        in(qml_engine, aInput->tag(), aInput->scope())->asyncCall("loadMain");
        if (qml_engine->rootObjects().isEmpty())
            return;

        if (prm.value("-md") == "TRUE"){
            rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
                aInput->out();
            }, rea::Json("name", "setDebugMode"));
            aInput->asyncCall("setDebugMode");
        }

        for (int i = 0; i < objs.size() - 1; ++i){
            objs[i]->setParent(objs[objs.size() - 1]);
            if (objs[i]->isWindowType()){
                auto w = reinterpret_cast<QWindow*>(objs[i]);
                w->setTransientParent(reinterpret_cast<QWindow*>(objs[objs.size() - 1]));
            }
        }
    }
    aInput->out();
}, rea::Json("name", "install1_qml"), "initRea");

QVariant qmlPipeline::tr(const QString& aOrigin){
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

}
