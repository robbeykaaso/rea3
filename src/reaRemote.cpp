#include "reaRemote.h"
#include "server.h"
#include "client.h"
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QProcess>

namespace rea {

pipelineRemote::pipelineRemote(const QString& aRemoteName, const QString& aLocalName) : pipeline(aRemoteName){
    m_localName = aLocalName;

    rea::pipeline::instance()->add<QJsonObject>([this](rea::stream<QJsonObject>* aInput){
        auto dt = aInput->data();
        if (dt.value("cmd") == "execute"){
            auto scp = aInput->scope();
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
                              dt.value("data"),
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
    if (aStream->scope()->data<bool>("remote")){
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
        QJsonObject dt;
        auto aData = aStream->QData();
        if (aData.type() == QVariant::Type::Map || aData.type() == QMetaType::QJsonObject){
            dt.insert("data", aData.toJsonObject());
        }else if (aData.type() == QVariant::Type::List || aData.type() == QMetaType::QJsonArray){
            dt.insert("data", aData.toJsonArray());
        }else if (aData.type() == QVariant::Type::String){
            dt.insert("data", aData.toString());
        }else if (aData.type() == QVariant::Type::Bool){
            dt.insert("data", aData.toBool());
        }else if (aData.type() == QVariant::Type::Double){
            dt.insert("data", aData.toDouble());
        }else if (aData.type() == QVariant::Type::Int){
            dt.insert("data", aData.toInt());
        }
        pipeline::instance()->run(name() + "_sendRemote",
                                  rea::Json(dt,
                                      "cmd", "execute",
                                      "name", aName,
                                      "remote", name(),
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
                                  "name", aName,
                                  "remote", "any"
                              ));
}

void pipelineRemote::executeFromRemote(const QString& aName, const QJsonValue& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag){
    if (aData.isObject())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData.toObject(), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isArray())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData.toArray(), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isString())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData.toString(), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isBool())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData.toBool(), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isDouble())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(aData.toDouble(), aTag, aScope), aSync, aNeedFuture, aFlag);
}

void pipelineRemote::removeFromRemote(const QString& aName){
    rea::pipeline::instance(m_localName)->remove(aName, false);
}

void pipelineQMLRemote::executeFromRemote(const QString& aName, const QJsonValue& aData, const QString& aTag, std::shared_ptr<scopeCache> aScope, const QJsonObject& aSync, bool aNeedFuture, const QString& aFlag){
    if (aData.isObject())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(QVariant::fromValue(aData.toObject()), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isArray())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(QVariant::fromValue(aData.toArray()), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isString())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(QVariant::fromValue(aData.toString()), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isBool())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(QVariant::fromValue(aData.toBool()), aTag, aScope), aSync, aNeedFuture, aFlag);
    else if (aData.isDouble())
        rea::pipeline::instance(m_localName)->execute(aName, rea::in(QVariant::fromValue(aData.toDouble()), aTag, aScope), aSync, aNeedFuture, aFlag);
}

void connectRemote(const QString& aLocal, const QString& aRemote, rea::pipeFunc<QJsonObject> aWriteRemote, bool aClient, const QString& aRemoteLocal){
    rea::pipeline::instance(aRemote);
    rea::pipeline::instance(aLocal)->updateOutsideRanges({aRemote});
    rea::pipeline::instance()->find("receiveFrom" + (aClient ? QString("Server") : "Client"))->next(aRemote + "_receiveRemote", (aRemoteLocal == "" ? aLocal : aRemoteLocal) + ";any");
    rea::pipeline::instance()->add<QJsonObject>(aWriteRemote, rea::Json("name", aRemote + "_sendRemote"));
}

QJsonObject connectRemoteConfig(const QString& aRole, const QJsonObject& aConfig, std::function<void(rea::stream<QJsonObject>*)> aWriteRemote){
    auto clt = aConfig.value(aRole).toObject();
    auto cnts = clt.value("connects").toArray();
    for (auto i : cnts){
        auto cnt = i.toObject();
        auto local = cnt.value("local").toString();
        auto remote = cnt.value("remote").toString();
        auto qml = cnt.value("qml").toBool();
        rea::pipeline::instance()->add<std::shared_ptr<rea::pipeline*>>([local, remote, qml](rea::stream<std::shared_ptr<rea::pipeline*>>* aInput){
            *aInput->data() = qml ? new rea::pipelineQMLRemote(remote, local) : new rea::pipelineRemote(remote, local);
        }, rea::Json("name", "create" + remote + "pipeline"));
        connectRemote(local, remote, aWriteRemote, aRole == "client", cnt.value("remoteLocal").toString());
    }
    return clt;
}

class processMan{
public:
    processMan(const QJsonArray& aList){
        for (auto i : aList){
            auto p = std::make_shared<QProcess>();
            auto nm = i.toString();
            std::cout << "start child process: " << nm.toStdString() << std::endl;
            p->start(nm);
            m_processes.push_back(p);
        }
    }
    ~processMan(){
        for (auto i : m_processes){
            if (i->state() == QProcess::Running)
                i->kill();
            }
    }
private:
    std::vector<std::shared_ptr<QProcess>> m_processes;
};

static rea::normalServer* server;
static rea::regPip<QQmlApplicationEngine*> init_tcp_linker([](rea::stream<QQmlApplicationEngine*>* aInput){
    QFile fl(QCoreApplication::applicationDirPath() + "/.rea");
    if (fl.open(QFile::ReadOnly)){
        auto cfg = QJsonDocument::fromJson(fl.readAll()).object();
        if (cfg.contains("client")){
            static rea::normalClient client;
            aInput->scope()->cache("client", &client);
            auto clt = connectRemoteConfig("client", cfg, [](rea::stream<QJsonObject>* aInput){
                client.sendServer(aInput);
            });
            if (clt.contains("ip") && clt.contains("port") && clt.contains("id"))
                rea::pipeline::instance()->run("tryLinkServer", rea::Json("ip", clt.value("ip"),
                                                                          "port", clt.value("port"),
                                                                          "id", clt.value("id")));
        }
        if (cfg.contains("server")){
            server = new rea::normalServer(cfg.value("server").toObject());
            aInput->scope()->cache("server", server);
            connectRemoteConfig("server", cfg, [](rea::stream<QJsonObject>* aInput){
                server->writeSocket(aInput->scope()->data<QTcpSocket*>("socket"), aInput->data());
            });
        }
        if (cfg.contains("process")){
            static processMan mn(cfg.value("process").toArray());
        }
    }
    aInput->out();
}, rea::Json("name", "install0_tcp"), "initRea");

}
