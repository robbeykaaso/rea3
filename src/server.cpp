#include "server.h"
#include <QTcpSocket>
#include <QJsonDocument>

namespace rea2 {

void normalServer::writeSocket(QTcpSocket* aSocket, const QJsonObject& aData){
    if (m_clients.contains(aSocket)){
        aSocket->write(QJsonDocument(aData).toJson(QJsonDocument::Compact));
        //aSocket->waitForBytesWritten();
        aSocket->flush();
        //std::cout << "send:" << aData.value("name").toString().toStdString() << "<<" << aSocket->bytesToWrite() << std::endl;
        while (aSocket->bytesToWrite() > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

normalServer::normalServer(const QJsonObject& aConfig) : QObject()
{
    m_pipeline = aConfig.value("pipeline").toString(getDefaultPipelineName());
    //QHostAddress add("127.0.0.1");
    connect(&m_socket,SIGNAL(newConnection()),this, SLOT(newConnect()));
    m_socket.listen(QHostAddress(aConfig.value("ip").toString("127.0.0.1")), aConfig.value("port").toInt(8081));

    rea2::pipeline::instance(m_pipeline)->add<QJsonObject, rea2::pipePartial>([](rea2::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea2::Json("name", "receiveFromClient"));

    rea2::pipeline::instance(m_pipeline)->add<bool>([](rea2::stream<bool>* aInput){
        aInput->out();
    }, rea2::Json("name", "clientStatusChanged"));
}

normalServer::~normalServer(){
    m_clients.clear();
}

void normalServer::disConnected(){
    QTcpSocket* client = static_cast<QTcpSocket*>(QObject::sender());  //https://stackoverflow.com/questions/25339943/how-to-know-if-a-client-connected-to-a-qtcpserver-has-closed-connection
    rea2::pipeline::instance(m_pipeline)->run("clientStatusChanged", false, "", std::make_shared<rea2::scopeCache>()->cache("socket", client));
    if (m_clients.contains(client)){
        m_clients.remove(client);
        disconnect(client,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽
        disconnect(client, SIGNAL(disconnected()), this, SLOT(disConnected()));
    }
}

void normalServer::newConnect()
{
    auto client = m_socket.nextPendingConnection(); //得到每个连进来的socket
    connect(client,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽
    /*connect(client,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, [this](QAbstractSocket::SocketError socketError) {
                std::cout << "Socket server error: "
                           << client->errorString().toStdString();
            });*/
    connect(client, SIGNAL(disconnected()), this, SLOT(disConnected()));
    rea2::pipeline::instance(m_pipeline)->run("clientStatusChanged", true, "", std::make_shared<rea2::scopeCache>()->cache("socket", client));
    m_clients.insert(client);
}

void normalServer::readMessage()	//读取信息
{
    auto client = static_cast<QTcpSocket*>(QObject::sender());
    while (client->bytesAvailable()) {
        QByteArray qba= client->readAll(); //读取
        QString ss=QVariant(qba).toString();
        //std::cout << ss.toStdString() << std::endl;
        auto strs = rea2::parseJsons(ss);
        for (auto str : strs){
            auto req = QJsonDocument::fromJson(str.toUtf8()).object();
            rea2::pipeline::instance(m_pipeline)->run<QJsonObject>("receiveFromClient", req, req.value("remote").toString(), std::make_shared<rea2::scopeCache>()->cache("socket", client));
            //std::cout << "outside: " << req.value("name").toString().toStdString() << std::endl;
        }
    }
}

}
