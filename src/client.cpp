#include "client.h"
#include <QJsonDocument>
#include <QNetworkProxy>

void normalClient::sendServer(rea::stream<QJsonObject>* aInput){
    if (!m_valid)
        return;
    m_socket.write(QJsonDocument(aInput->data()).toJson(QJsonDocument::Compact));
    m_socket.flush();  //waitForBytesWritten
    while (m_socket.bytesToWrite() > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

normalClient::normalClient(const QJsonObject& aConfig) : QObject()
{
    connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(ReceiveState(QAbstractSocket::SocketState)));

    rea::pipeline::instance()->add<QJsonObject, rea::pipePartial>([](rea::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea::Json("name", "receiveFromServer"));

    rea::pipeline::instance()->add<QJsonObject>([this](rea::stream<QJsonObject>* aInput){
        m_socket.abort();
        auto dt = aInput->data();
        if (dt.contains("ip") && dt.contains("port") && dt.contains("id"))
            ServerFound(dt.value("ip").toString(), dt.value("port").toString(), dt.value("id").toString());
        aInput->out();
    }, rea::Json("name", "tryLinkServer"));

    rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea::Json("name", "clientBoardcast"));
}

normalClient::~normalClient(){
    m_socket.disconnectFromHost();
    //m_socket.abort();
}

void normalClient::RegistOnStateChanged(std::function<void(QAbstractSocket::SocketState)> aEvent){
    connect(&m_socket, &QAbstractSocket::stateChanged, aEvent);
}

void normalClient::ServerFound(QString aIP, QString aPort, QString aID)
{
    m_detail = aIP + ":" + aPort + ":" + aID;
    rea::pipeline::instance()->run<QJsonObject>("clientBoardcast", rea::Json("value", "connect socket", "detail", m_detail));
    m_socket.abort();
    m_socket.setProxy(QNetworkProxy::NoProxy);
    //m_socket.setSocketOption()
    m_socket.connectToHost(aIP, aPort.toInt());
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
}

void normalClient::ReceiveState(QAbstractSocket::SocketState aState){
    if (aState == QAbstractSocket::SocketState::ConnectedState){
        m_valid = true;
        rea::pipeline::instance()->run<QJsonObject>("clientBoardcast", rea::Json("value", "socket connected", "detail", m_detail, "host", m_socket.localAddress().toString()));
        connected();
    }else if (aState == QAbstractSocket::SocketState::UnconnectedState){
        m_valid = false;
        rea::pipeline::instance()->run<QJsonObject>("clientBoardcast", rea::Json("value", "socket unconnected"));
        disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
    }
}

void normalClient::ReceiveMessage()
{
    QByteArray qba = m_socket.readAll();
    QString ss = QVariant(qba).toString();
    auto strs = rea::parseJsons(ss);
    for (auto msg : strs){
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        auto res = doc.object();
        rea::pipeline::instance()->run<QJsonObject>("receiveFromServer", res, res.value("remote").toString());
    }
}
