#include "rea.h"
#include <QJSValue>
#include <QQmlEngine>

namespace rea {

class pipelineQML : public pipeline{
public:
    Q_OBJECT
public:
    pipelineQML();
protected:
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFromOutside = false) override;
    void tryExecutePipeOutside(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, const QString& aFlag) override;
};

class qmlScopeCache : public QObject{
    Q_OBJECT
public:
    qmlScopeCache(std::shared_ptr<scopeCache> aScope);
    Q_INVOKABLE QJSValue cache(const QString& aName, QJSValue aData);
    Q_INVOKABLE QJSValue data(const QString& aName);
private:
    std::shared_ptr<scopeCache> m_scope;
};

class qmlStream : public QObject{
    Q_OBJECT
public:
    qmlStream(){}
    qmlStream(std::shared_ptr<stream<QJSValue>> aStream){
        m_stream = aStream;
    }
    ~qmlStream(){

    }
    Q_INVOKABLE QJSValue setData(QJSValue aData);
    Q_INVOKABLE QJSValue scope(bool aNew = false);
    Q_INVOKABLE QJSValue data();
    Q_INVOKABLE QString tag();
    Q_INVOKABLE QJSValue out(const QString& aTag = "");
    Q_INVOKABLE QJSValue outs(QJSValue aOut, const QString& aNext = "", const QString& aTag = "");
    Q_INVOKABLE QJSValue asyncCall(const QString& aName, const QString& aPipeline = "qml");
    Q_INVOKABLE QJSValue asyncCallF(QJSValue aFunc, const QJsonObject& aParam = QJsonObject(), const QString& aPipeline = "qml");
private:
    std::shared_ptr<stream<QJSValue>> m_stream;
};

class qmlPipe : public QObject
{
    Q_OBJECT
public:
    qmlPipe(pipeline* aParent, const QString& aName);
    ~qmlPipe(){

    }
public:
    Q_INVOKABLE QString actName() {return m_name;}
    Q_INVOKABLE void resetTopo();
    Q_INVOKABLE QJSValue next(const QString& aName, const QString& aTag = "");
    Q_INVOKABLE QJSValue nextB(const QString& aName, const QString& aTag = "");
    Q_INVOKABLE QJSValue nextF(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject());
    Q_INVOKABLE QJSValue nextFB(QJSValue aFunc, const QString& aTag = "", const QJsonObject& aParam = QJsonObject());
    Q_INVOKABLE void removeNext(const QString& aName);
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
    static Q_INVOKABLE void run(const QString& aName, const QJSValue& aInput, const QString& aTag = "", const QJsonObject& aScopeCache = QJsonObject());
    static Q_INVOKABLE void call(const QString& aName, const QJSValue& aInput);
    static Q_INVOKABLE QJSValue input(const QJSValue& aInput, const QString& aTag = "", const QJsonObject& aScopeCache = QJsonObject());
    static Q_INVOKABLE void remove(const QString& aName);
    static Q_INVOKABLE QJSValue add(QJSValue aFunc, const QJsonObject& aParam = QJsonObject());
    static Q_INVOKABLE QJSValue find(const QString& aName);
    static Q_INVOKABLE QJSValue asyncCall(const QString& aName, const QJSValue& aInput);
    static Q_INVOKABLE QVariant tr(const QString& aOrigin);
};

DSTDLL QString tr0(const QString& aOrigin);

}
