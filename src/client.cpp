#include "client.h"
#include <QJsonDocument>
#include <QNetworkProxy>

namespace rea2 {

void normalClient::sendServer(rea2::stream<QJsonObject>* aInput){
    if (!m_valid)
        return;
    m_socket.write(QJsonDocument(aInput->data()).toJson(QJsonDocument::Compact));
    //m_socket.waitForBytesWritten();
    m_socket.flush();  //waitForBytesWritten
    while (m_socket.bytesToWrite() > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

normalClient::normalClient(const QJsonObject& aConfig) : QObject()
{
    m_pipeline = aConfig.value("pipeline").toString(getDefaultPipelineName());
    connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(ReceiveState(QAbstractSocket::SocketState)));

    rea2::pipeline::instance(m_pipeline)->add<QJsonObject, rea2::pipePartial>([](rea2::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea2::Json("name", "receiveFromServer"));

    rea2::pipeline::instance(m_pipeline)->add<QJsonObject>([this](rea2::stream<QJsonObject>* aInput){
        m_socket.abort();
        auto dt = aInput->data();
        if (dt.contains("ip") && dt.contains("port") && dt.contains("id"))
            ServerFound(dt.value("ip").toString(), dt.value("port").toString(), dt.value("id").toString());
        aInput->out();
    }, rea2::Json("name", "tryLinkServer"));

    rea2::pipeline::instance(m_pipeline)->add<QJsonObject>([](rea2::stream<QJsonObject>* aInput){
        aInput->out();
    }, rea2::Json("name", "clientBoardcast"));
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
    rea2::pipeline::instance(m_pipeline)->run<QJsonObject>("clientBoardcast", rea2::Json("value", "connect socket", "detail", m_detail));
    m_socket.abort();
    m_socket.setProxy(QNetworkProxy::NoProxy);
    //m_socket.setSocketOption()
    m_socket.connectToHost(aIP, aPort.toInt());
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
}

void normalClient::ReceiveState(QAbstractSocket::SocketState aState){
    if (aState == QAbstractSocket::SocketState::ConnectedState){
        m_valid = true;
        rea2::pipeline::instance(m_pipeline)->run<QJsonObject>("clientBoardcast", rea2::Json("value", "socket connected", "detail", m_detail, "host", m_socket.localAddress().toString()));
        connected();
    }else if (aState == QAbstractSocket::SocketState::UnconnectedState){
        m_valid = false;
        rea2::pipeline::instance(m_pipeline)->run<QJsonObject>("clientBoardcast", rea2::Json("value", "socket unconnected"));
        disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
    }
}

void normalClient::ReceiveMessage()
{
    while (m_socket.bytesAvailable()) {
        QByteArray qba = m_socket.readAll();
        QString ss = QVariant(qba).toString();
        auto strs = rea2::parseJsons(ss);
        for (auto msg : strs){
            QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
            auto res = doc.object();
            //std::cout << "receive:" << res.value("name").toString().toStdString() << "<<" << strs.size() << std::endl;
            rea2::pipeline::instance(m_pipeline)->run<QJsonObject>("receiveFromServer", res, res.value("remote").toString());
        }
    }
}

}
