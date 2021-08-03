#include "rea.h"
#include <QJSValue>
#include <QQmlEngine>

namespace rea {

class pipelineQML : public pipeline{
public:
    Q_OBJECT
public:
    pipelineQML();
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFutureNeed = false, const QString& aFrom = "") override;
protected:
    void removePipeOutside(const QString& aName) override;
    void tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag) override;
    bool externalNextGot(pipe0* aPipe, std::shared_ptr<stream0> aStream, const QString& aFrom) override;
};

class pipelineQMLJS : public pipeline{
public:
    Q_OBJECT
public:
    pipelineQMLJS();
public:
    Q_INVOKABLE void executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag);
    Q_INVOKABLE void removeFromJS(const QString& aName);
    void remove(const QString& aName, bool) override;
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFutureNeed = false, const QString& aFrom = "") override;
signals:
    void executeJSPipe(const QString& aName, const QVariant& aData, const QString& aTag, const QVariantList& aScope, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom);
    void removeJSPipe(const QString& aName);
};

class qmlPipeline;

class qmlScopeCache : public QObject{
    Q_OBJECT
public:
    qmlScopeCache(std::shared_ptr<scopeCache> aScope);
    ~qmlScopeCache();
    Q_INVOKABLE QJSValue cache(const QString& aName, QJSValue aData);
    Q_INVOKABLE QJSValue data(const QString& aName);
private:
    std::shared_ptr<scopeCache> m_scope;
    friend qmlPipeline;
};

class DSTDLL qmlStream : public QObject{
    Q_OBJECT
public:
    qmlStream();
    qmlStream(std::shared_ptr<stream<QVariant>> aStream);
    ~qmlStream();
    Q_INVOKABLE QJSValue setData(QJSValue aData);
    Q_INVOKABLE QJSValue scope(bool aNew = false);
    Q_INVOKABLE QJSValue data();
    Q_INVOKABLE QString tag();
    Q_INVOKABLE QJSValue out(const QString& aTag = "");
    Q_INVOKABLE QJSValue outs(QJSValue aOut, const QString& aNext = "", const QString& aTag = "");
    Q_INVOKABLE QJSValue outsB(QJSValue aOut, const QString& aNext = "", const QString& aTag = "");
    Q_INVOKABLE void noOut();
    Q_INVOKABLE QJSValue asyncCall(const QString& aName, bool aEventLevel = true, bool aOutside = false);
    Q_INVOKABLE QJSValue asyncCallF(QJSValue aFunc, const QJsonObject& aParam = QJsonObject(), bool aEventLevel = true);
private:
    std::shared_ptr<stream<QVariant>> m_stream;
};

DSTDLL QJSValue qmlstream2JSValue(qmlStream* aStream);

template <>
class funcType<QVariant, QJSValue>{
public:
    void doEvent(QJSValue aFunc, std::shared_ptr<stream<QVariant>> aStream){
        if (!aFunc.equals(QJSValue::NullValue)){
            QJSValueList paramlist;
            qmlStream stm(aStream);
            paramlist.append(qmlstream2JSValue(&stm));
            aFunc.call(paramlist);
        }
    }
};

class qmlPipe : public QObject
{
    Q_OBJECT
public:
    qmlPipe(pipeline* aParent, const QString& aName);
    ~qmlPipe();
public:
    Q_INVOKABLE QString actName() {return m_name;}
    Q_INVOKABLE QJSValue next(const QString& aName, const QString& aTag = "");
    Q_INVOKABLE QJSValue nextB(const QString& aName, const QString& aTag = "");
    Q_INVOKABLE QJSValue nextF(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject());
    Q_INVOKABLE QJSValue nextFB(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject());
    Q_INVOKABLE void removeNext(const QString& aName, bool aAndDelete = false, bool aOutside = true);
    Q_INVOKABLE void removeAspect(const QString& aType, const QString& aAspect = "");
private:
    pipeline* m_parent;
    QString m_name;
    QJsonObject m_param;
};

class qmlPipeline : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(qmlPipeline)
public:
    qmlPipeline();
    ~qmlPipeline();
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
public:
    static Q_INVOKABLE QJSValue run(const QString& aName, const QJSValue& aInput, const QString& aTag = "", const QJSValue& aScope = QJSValue::NullValue);
    static Q_INVOKABLE QJSValue call(const QString& aName, const QJSValue& aInput, const QJsonObject& aScope = QJsonObject(), bool aAOP = true);
    static Q_INVOKABLE QJSValue input(const QJSValue& aInput, const QString& aTag = "", const QJsonObject& aScopeCache = QJsonObject(), bool aAutoTag = false);
    static Q_INVOKABLE void remove(const QString& aName, bool aOutside = false);
    static Q_INVOKABLE QJSValue add(QJSValue aFunc, const QJsonObject& aParam = QJsonObject());
    static Q_INVOKABLE QJSValue find(const QString& aName);
    static Q_INVOKABLE QJSValue asyncCall(const QString& aName, const QJSValue& aInput, bool aEventLevel = true, bool aOutside = false);
    static Q_INVOKABLE QString tr(const QString& aOrigin);
};

DSTDLL QString tr0(const QString& aOrigin);

#define regQMLPipe(aType) \
static regPip<QString> reg_qml_pipe##aType([](stream<QString>* aInput){ \
    auto sp = aInput->scope(); \
    aInput->setData(pipeline::instance(getDefaultQMLPipelineName()) \
                    ->add<QVariant, pipe##aType, QJSValue, QJSValue>( \
                        sp->data<QJSValue>("func"), \
                        sp->data<QJsonObject>("param")) \
                    ->actName()); \
}, rea::Json("name", STR(createQMLPipe##aType)));

}
