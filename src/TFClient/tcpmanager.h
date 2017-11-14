#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

class TCPManager : public QObject
{
    Q_OBJECT
public:
    TCPManager();
    void connectTo(const QString&);
    void disconnect(const QString&);

public slots:
    void onPushUpdate(/*something here*/);
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket sock;
    uint port;
    QDataStream* data;

private slots:
    void onReadyRead();

signals:
    void updateReceived(QDataStream*);
    void mapReceived(QDataStream*);
    void idReceived(char);
    void disconnected(const QString&);
};

#endif // TCPMANAGER_H
