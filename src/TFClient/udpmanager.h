#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QDataStream>

class UDPManager : public QObject
{
    Q_OBJECT
public:
    UDPManager();
    ~UDPManager();
    void connectTo(const QString&, QString);
    void disconnect(const QString&);

public slots:
    void onPushUpdate(const QMap<int, bool>& status, float ang, bool clicked);

private:
    QUdpSocket sock;
    QHostAddress address;
    uint sport;
    uint cport;
    QString name;

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);

signals:
    void updateReceived(QDataStream*);
    void error(QString);
};

#endif // TCPMANAGER_H
