#ifndef REAL_FRAMEWORK_SERVER_H_
#define REAL_FRAMEWORK_SERVER_H_

#include "reaRemote.h"
#include <QTcpServer>

namespace rea {

class DSTDLL normalServer : public QObject{
    Q_OBJECT
public:
    normalServer(const QJsonObject& aConfig = QJsonObject());
    ~normalServer();
public slots:
    void newConnect();
    void disConnected();
    void readMessage();
    void writeSocket(QTcpSocket* aSocket, const QJsonObject& aData);
private:
    QTcpServer m_socket;
    QSet<QTcpSocket*> m_clients;
    QString m_pipeline;
};

}



#endif
