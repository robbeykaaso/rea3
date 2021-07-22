#include "server.h"
#include <QTcpSocket>
#include <QJsonDocument>

void normalServer::writeSocket(QTcpSocket* aSocket, const QJsonObject& aData){
    if (m_clients.contains(aSocket)){
        aSocket->write(QJsonDocument(aData).toJson(QJsonDocument::Compact));
        aSocket->flush();
        while (aSocket->bytesToWrite() > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

normalServer::normalServer(const QJsonObject& aConfig) : QObject()
{
    //QHostAddress add("127.0.0.1");
    socket_.listen(QHostAddress::LocalHost, 8081);
    connect(&socket_,SIGNAL(newConnection()),this,SLOT(NewConnect()));

    rea::pipeline::instance()->add<QJsonObject, rea::pipePartial>([](rea::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea::Json("name", "receiveFromClient"));

    rea::pipeline::instance()->add<bool>([](rea::stream<bool>* aInput){
        aInput->out();
    }, rea::Json("name", "clientStatusChanged"));
}

normalServer::~normalServer(){
    m_clients.clear();
}

void normalServer::DisConnected(){
    QTcpSocket* client = static_cast<QTcpSocket*>(QObject::sender());  //https://stackoverflow.com/questions/25339943/how-to-know-if-a-client-connected-to-a-qtcpserver-has-closed-connection
    rea::pipeline::instance()->run("clientStatusChanged", false, "", std::make_shared<rea::scopeCache>()->cache("socket", client));
    if (m_clients.contains(client)){
        m_clients.remove(client);
        disconnect(client,SIGNAL(readyRead()),this,SLOT(ReadMessage())); //有可读的信息，触发读函数槽
        disconnect(client, SIGNAL(disconnected()), this, SLOT(DisConnected()));
    }
}

void normalServer::NewConnect()
{
    auto client = socket_.nextPendingConnection(); //得到每个连进来的socket
    connect(client,SIGNAL(readyRead()),this,SLOT(ReadMessage())); //有可读的信息，触发读函数槽
    /*connect(client,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, [this](QAbstractSocket::SocketError socketError) {
                std::cout << "Socket server error: "
                           << client->errorString().toStdString();
            });*/
    connect(client, SIGNAL(disconnected()), this, SLOT(DisConnected()));
    rea::pipeline::instance()->run("clientStatusChanged", true, "", std::make_shared<rea::scopeCache>()->cache("socket", client));
    m_clients.insert(client);
}

void normalServer::ReadMessage()	//读取信息
{
    auto client = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray qba= client->readAll(); //读取
    QString ss=QVariant(qba).toString();

    //std::cout << ss.toStdString() << std::endl;

    auto strs = rea::parseJsons(ss);
    for (auto str : strs){
        auto req = QJsonDocument::fromJson(str.toUtf8()).object();
        rea::pipeline::instance()->run<QJsonObject>("receiveFromClient", req, req.value("remote").toString(), std::make_shared<rea::scopeCache>()->cache("socket", client));
        //std::cout << "outside: " << req.value("name").toString().toStdString() << std::endl;
    }
}
