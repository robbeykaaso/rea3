#ifndef REAREMOTE_H_
#define REAREMOTE_H_

#include "rea.h"

namespace rea {

class streamRemote{
public:
    streamRemote(){}
    streamRemote(std::shared_ptr<stream<QJsonObject>> aData){
        data = aData;
    }
    std::shared_ptr<stream<QJsonObject>> data;
};


class DSTDLL pipelineRemote : public rea::pipeline{
    Q_OBJECT
public:
    pipelineRemote(const QString& aRemoteName, const QString& aLocalName);
signals:
    void sendRemote(rea::streamRemote aInput);
private:
    void executeFromRemote(const QString& aName, const QJsonObject& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag);
    void removeFromRemote(const QString& aName);
public slots:
    void receiveRemote(rea::streamRemote aInput);
public:
    void remove(const QString& aName, bool) override;
    void execute(const QString& aName, std::shared_ptr<rea::stream0> aStream, const QJsonObject& aSync = QJsonObject(), bool aFutureNeed = false, const QString& aFrom = "") override;
private:
    QString m_localName;
};

}

Q_DECLARE_METATYPE(rea::streamRemote)

#endif
