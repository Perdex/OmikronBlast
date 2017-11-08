#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class QHostAddress;

class TCPManager: public QObject
{
    Q_OBJECT
public:
    TCPManager();
    ~TCPManager();
    QString getAddress();
    quint16 getPort();
    QVector<QString> readData();
    void sendData();
    void addData(QString data);
private slots:
    void newClient();
private:
    QVector<QTcpSocket*> clients;
    QTcpServer *server;
    uint port;
    QVector<QString> data;
};

#endif // TCPMANAGER_H
