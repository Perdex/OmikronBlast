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
    void connectTo(const QString&, const int);
    void disconnect(const QString&);

public slots:
    void onPushUpdate(const QMap<int, bool>& status, float ang, bool clicked);
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket sock;
    uint port;
    QDataStream* data;

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);

signals:
    void updateReceived(QDataStream*);
    void mapReceived(/*Map&*/);
    void idReceived(qint8);
    void disconnected(const QString&);
    void connected();
    void error(QString);
};

#endif // TCPMANAGER_H
