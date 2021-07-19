#include "rea.h"

namespace rea{

class DSTDLL pipelineJS : public pipeline{
public:
    Q_OBJECT
public:
    pipelineJS(const QString& aName = "js");
public:
    Q_INVOKABLE void executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag);
    Q_INVOKABLE void removeFromJS(const QString& aName);
    void remove(const QString& aName, bool) override;
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFutureNeed = false, const QString& aFrom = "") override;
signals:
    void executeJSPipe(const QString& aName, const QVariant& aData, const QString& aTag, const QVariantList& aScope, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom);
    void removeJSPipe(const QString& aName);
};

class DSTDLL environmentJS : public QObject{
public:
    Q_OBJECT
public:
    Q_PROPERTY(QJsonObject env READ getEnv WRITE setEnv NOTIFY envChanged)
signals:
    void envChanged();
private:
    void setEnv(const QJsonObject& aEnvs);
    QJsonObject getEnv();
    QJsonObject m_envs;
};

//=>pipelineCPlugMirror
//=>pipelineQMLMirror

}
