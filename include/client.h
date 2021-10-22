#ifndef REAL_FRAMEWORK_CLIENT_H_
#define REAL_FRAMEWORK_CLIENT_H_

#include "rea.h"
#include <QTcpSocket>
#include <QTimer>

namespace rea2 {

class DSTDLL normalClient : public QObject{
    Q_OBJECT
public:
    normalClient(const QJsonObject& aConfig = QJsonObject());
    ~normalClient();
    void RegistOnStateChanged(std::function<void(QAbstractSocket::SocketState)> aEvent);
signals:
    void connected();
public slots:
    void ServerFound(QString aIP, QString aPort, QString aID);
    void ReceiveState(QAbstractSocket::SocketState aState);
    void ReceiveMessage();
    void sendServer(rea2::stream<QJsonObject>* aInput);
private:
    QTcpSocket m_socket;
    bool m_valid = false;
    QString m_detail;
    QString m_pipeline;
};

}

#endif
