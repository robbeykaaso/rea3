#include "reaRemote.h"
#include <QQmlApplicationEngine>

namespace rea {

pipelineRemote::pipelineRemote(const QString& aRemoteName, const QString& aLocalName) : pipeline(aRemoteName){
    m_localName = aLocalName;

    rea::pipeline::instance()->add<QJsonObject>([this](rea::stream<QJsonObject>* aInput){
        auto dt = aInput->data();
        if (dt.value("cmd") == "execute"){
            auto scp = aInput->scope();
            if (!scp)
                scp = std::make_shared<scopeCache>();
            auto scp_arr = dt.value("scope").toArray();
            for (int i = 0; i < scp_arr.size(); i += 2){
                if (scp_arr[i + 1].isString())
                    scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toString());
                else if (scp_arr[i + 1].isBool())
                    scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toBool());
                else if (scp_arr[i + 1].isDouble())
                    scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toDouble());
                else if (scp_arr[i + 1].isArray())
                    scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toArray());
                else if (scp_arr[i + 1].isObject())
                    scp->cache(scp_arr[i].toString(), scp_arr[i + 1].toObject());
            }
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
    }, rea::Json("name", name() + "_receiveRemote"));
};

void pipelineRemote::execute(const QString& aName, std::shared_ptr<stream0> aStream, const QJsonObject& aSync, bool aFutureNeed, const QString& aFrom){
    auto aData = aStream->QData();
    if (aStream->scope()->data<bool>("remote") && (aData.type() == QVariant::Type::Map || aData.type() == QMetaType::QJsonObject)){
        auto lst = aStream->scope()->toList();
        QJsonArray scp;
        for (auto i = 0; i < lst.size(); i += 2){
            auto tp = lst[i + 1].type();
            if (tp == QVariant::Type::String){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toString());
            }else if (tp == QVariant::Type::Bool){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toBool());
            }else if (tp == QVariant::Type::Double){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toDouble());
            }else if (tp == QVariant::Type::List){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toJsonArray());
            }else if (tp == QVariant::Type::Map){
                scp.push_back(lst[i].toString());
                scp.push_back(lst[i + 1].toJsonObject());
            }
        }
        pipeline::instance()->run(name() + "_sendRemote",
                                  rea::Json(
                                      "cmd", "execute",
                                      "name", aName,
                                      "data", aData.toJsonObject(),
                                      "tag", aStream->tag(),
                                      "scope", scp,
                                      "sync", aSync,
                                      "needFuture", aFutureNeed,
                                      "from", aFrom == "" ? name() : aFrom
                                  ), aStream->tag(), aStream->scope());
    }
}

void pipelineRemote::remove(const QString& aName, bool){
    pipeline::instance()->run(name() + "_sendRemote",
                              rea::Json(
                                  "cmd", "remove",
                                  "name", aName
                              ));
}

void pipelineRemote::executeFromRemote(const QString& aName, const QJsonObject& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag){
    rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData, aTag, aScope), aSync, aNeedFuture, aFlag);
}

void pipelineRemote::removeFromRemote(const QString& aName){
    rea::pipeline::instance(m_localName)->remove(aName, false);
}

/*static regPip<QQmlApplicationEngine*> reg_recative2_remote([](stream<QQmlApplicationEngine*>* aInput){
    auto cfg = aInput->scope()->data<QJsonObject>("rea-qml");
    if (cfg.value("use").toBool(true)){
        qmlRegisterType<streamRemote>("StreamRemote", 1, 0, "StreamRemote");
    }
    aInput->out();
}, rea::Json("name", "install2_remote"), "initRea");
*/
}
