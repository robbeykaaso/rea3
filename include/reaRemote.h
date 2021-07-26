#ifndef REA_REMOTE_H_
#define REA_REMOTE_H_

#include "rea.h"

namespace rea {

class DSTDLL pipelineRemote : public rea::pipeline{
public:
    pipelineRemote(const QString& aRemoteName, const QString& aLocalName);
protected:
    virtual void executeFromRemote(const QString& aName, const QJsonValue& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag);
private:
    void removeFromRemote(const QString& aName);
public:
    void remove(const QString& aName, bool) override;
    void execute(const QString& aName, std::shared_ptr<rea::stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFutureNeed = false, const QString& aFrom = "") override;
protected:
    QString m_localName;
};

class DSTDLL pipelineQMLRemote : public pipelineRemote{
public:
    pipelineQMLRemote(const QString& aRemoteName, const QString& aLocalName) : pipelineRemote(aRemoteName, aLocalName){

    }
protected:
    virtual void executeFromRemote(const QString& aName, const QJsonValue& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag);
};

DSTDLL void connectRemote(const QString& aLocal, const QString& aRemote, rea::pipeFunc<QJsonObject> aWriteRemote, bool aClient = true, const QString& aRemoteLocal = "");

}

#endif
