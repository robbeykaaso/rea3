#include "reaRemote.h"
#include <QQmlApplicationEngine>

namespace rea {

pipelineRemote::pipelineRemote(const QString& aRemoteName, const QString& aLocalName) : pipeline(aRemoteName){
    m_localName = aLocalName;
};

void pipelineRemote::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    auto aData = aStream->QData();
    if (aData.type() == QVariant::Type::Map || aData.type() == QMetaType::QJsonObject){
        auto lst = aStream->scope()->toList();
        QJsonArray scp;
        for (auto i = 0; i < lst.size(); i += 2)
            if (lst[i + 1].type() == QVariant::Type::String){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toString());
            }
        auto stm = std::make_shared<stream<QJsonObject>>(rea::Json(
                                                             "cmd", "execute",
                                                             "name", aName,
                                                             "data", aData.toJsonObject(),
                                                             "tag", aStream->tag(),
                                                             "scope", scp,
                                                             "sync", aSync,
                                                             "needFuture", aFutureNeed,
                                                             "from", aFrom == "" ? name() : aFrom
                                                             ), aStream->tag(), aStream->scope());
        sendRemote(streamRemote(stm));
    }
}

void pipelineRemote::remove(const QString& aName, bool){
    auto stm = std::make_shared<stream<QJsonObject>>(rea::Json(
                                                         "cmd", "remove",
                                                         "name", aName
                                                         ));
    sendRemote(streamRemote(stm));
}

void pipelineRemote::receiveRemote(streamRemote aInput){
    auto dt = aInput.data->data();
    if (dt.value("cmd") == "execute"){
        auto scp = aInput.data->scope();
        auto scp_arr = dt.value("scope").toArray();
        for (int i = 0; i < scp_arr.size(); i += 2)
            scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toString());
        executeFromRemote(dt.value("name").toString(),
                          dt.value("data").toObject(),
                          dt.value("tag").toString(),
                          scp,
                          dt.value("sync").toObject(),
                          dt.value("needFuture").toBool(),
                          dt.value("from").toString());
    }else if (dt.value("cmd") == "remove"){
        removeFromRemote(dt.value("name").toString());
    }
}

void pipelineRemote::executeFromRemote(const QString& aName, const QJsonObject& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag){
    rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData, aTag, aScope), aSync, aNeedFuture, aFlag);
}

void pipelineRemote::removeFromRemote(const QString& aName){
    rea::pipeline::instance(m_localName)->remove(aName, false);
}

static regPip<QQmlApplicationEngine*> reg_recative2_qml([](stream<QQmlApplicationEngine*>* aInput){
    std::cout << qRegisterMetaType<streamRemote>() << std::endl;
    aInput->out();
}, rea::Json("name", "install0_remote"), "initRea");

}
