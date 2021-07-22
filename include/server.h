#ifndef REAL_FRAMEWORK_SERVER_H_
#define REAL_FRAMEWORK_SERVER_H_

#include "reaRemote.h"
#include <QTcpServer>

class DSTDLL normalServer : public QObject{
    Q_OBJECT
public:
    normalServer(const QJsonObject& aConfig = QJsonObject());
    ~normalServer();
public slots:
    void NewConnect();
    void DisConnected();
    void ReadMessage();
    void writeSocket(QTcpSocket* aSocket, const QJsonObject& aData);
private:
    QTcpServer socket_;
    QSet<QTcpSocket*> m_clients;
};

#endif
