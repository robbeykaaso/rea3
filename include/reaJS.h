#include "rea.h"

namespace rea{

class pipelineJS : public pipeline{
public:
    Q_OBJECT
public:
    pipelineJS();
public:
    Q_INVOKABLE void tryExecuteOutsidePipe(const QString& aName, const QVariant& aData, const QString& aTag, const QJsonObject& aScope, const QJsonObject& aSync, const QString& aFlag);
    Q_INVOKABLE void removePipeOutside(const QString& aName) override;
    void remove(const QString& aName, bool) override;
signals:
    void executeJSPipe(const QString& aName, const QVariant& aData, const QString& aTag, const QVariantList& aScope, const QJsonObject& aSync, bool aFromOutside);
    void removeJSPipe(const QString& aName);
protected:
    void execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFromOutside = false) override;
};

}
