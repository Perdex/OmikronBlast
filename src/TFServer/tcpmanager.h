#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class QHostAddress;
class MainWindow;

class TCPManager: public QObject
{
    Q_OBJECT
public:
    TCPManager(MainWindow* mainWindow);
    ~TCPManager();
    QString getAddress();
    QString getPort();
    TCPManager *operator<<(stuff s);
    void flush();
private slots:
    void newClient();
private:
    QVector<QTcpSocket*> clients;
    QTcpServer *server;
    uint port;
    MainWindow* mainWindow;
};

#endif // TCPMANAGER_H
