#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QVector>
#include <QHostAddress>

class QUdpServer;
class QUdpSocket;
class MainWindow;
class stuff;
class Message;
class QNetworkDatagram;

class UDPManager: public QObject
{
    Q_OBJECT
public:
    UDPManager(MainWindow* mainWindow);
    ~UDPManager();
    QString getAddress();
    UDPManager &operator<<(Message* msg);
    void gameStarted();
    void processStatus(QDataStream &stream, QNetworkDatagram &datagram);
    void processHandshake(QDataStream &stream, QNetworkDatagram &datagram);
private slots:
    void readData();
private:
    QUdpSocket* socket;
    uint sport;
    uint cport;
    MainWindow* mainWindow;
    QVector<QHostAddress> addresses;
};

#endif // TCPMANAGER_H
