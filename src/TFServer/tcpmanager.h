#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class QHostAddress;
class MainWindow;
class stuff;

class TCPManager: public QObject
{
    Q_OBJECT
public:
    TCPManager(MainWindow* mainWindow);
    ~TCPManager();
    QString getAddress();
    QString getPort();
    TCPManager &operator<<(stuff*);
    TCPManager *operator<<(QString);
    void flush();
    friend TCPManager& operator<<(TCPManager &tcp, const stuff &s);
private slots:
    void newClient();
private:
    QVector<QTcpSocket*> clients;
    QTcpServer *server;
    uint port;
    MainWindow* mainWindow;
};

#endif // TCPMANAGER_H
