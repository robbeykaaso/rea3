#include "rea.h"

namespace rea{

class pipelineJS : public pipeline{
public:
    Q_OBJECT
public:
    pipelineJS();
public:
    Q_INVOKABLE void executeFromJS(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, const QString& aFlag);
    Q_INVOKABLE void removeFromJS(const QString& aName);
    void remove(const QString& aName, bool) override;
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFromOutside = false) override;
signals:
    void executeJSPipe(const QString& aName, const QVariant& aData, const QString& aTag, const QVariantList& aScope, const QJsonObject& aSync, bool aFromOutside);
    void removeJSPipe(const QString& aName);
};

//=>pipelineCPlugMirror
//=>pipelineQMLMirror

}
