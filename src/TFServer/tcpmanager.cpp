#include "tcpmanager.h"
#include "mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>

#include "stuff.h"

TCPManager::TCPManager(MainWindow* mainWindow)
    : clients(),
      server(nullptr),
      port(2000),
      mainWindow(mainWindow)
{

    server = new QTcpServer();
    if (!server->listen()) {
        QMessageBox::critical(NULL, QString("TFServer"),
                              QString("Unable to start the server: %1.")
                              .arg(server->errorString()));
        server->close();
        return;
    }

    QObject::connect(server, &QTcpServer::newConnection, this, &TCPManager::newClient);
/*
 * Can be used for debugging
 * Copied from example
 *
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QMessageBox::information(NULL, QString("Server status"),
                          QString("The server is running on\n\nIP: %1\nport: %2\n\n")
                         .arg(ipAddress).arg(server->serverPort()));
*/
}

TCPManager::~TCPManager()
{
    if(server != nullptr){
        delete server;
        server = nullptr;
    }

}

QString TCPManager::getAddress(){
    if(server != nullptr)
        return server->serverAddress().toString();
    return QString();
}
QString TCPManager::getPort(){
    if(server != nullptr)
        return QString("%1").arg(server->serverPort());
    return 0;
}

void TCPManager::newClient(){

    auto socket = server->nextPendingConnection();
    clients.push_back(socket);
    //QObject::connect(socket, &QTcpSocket::channelReadyRead, this, receiveData);
    qDebug() << "ASDF client found!!1\n";

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);

    qint16 id = clients.size();
    QString ver = "TFGAME-SERVER";

    stream << ver << id;

    socket->write(msg);
    socket->flush();

    //QString s;
    //for(auto client: clients){
    //    s += client->peerName();
    //    s += '\n';
    //}
    mainWindow->setPlayersText(QString("Number connected: %1").arg(clients.length()));

    QDataStream *s = new QDataStream(socket);
    //s->setDevice(socket);
    qDebug() << s->device();
    s->setVersion(QDataStream::Qt_5_9);
    mainWindow->addPlayer(id, s);
}
/*
QVector<QString> TCPManager::readData(){
    QVector<QString> receivedData;
    for(auto client: clients){
        QByteArray ba = client->readAll();
        if(!ba.isEmpty())
            receivedData += QString(ba);

    }
    return receivedData;
}

void TCPManager::sendData(){

    for(auto client: clients){
        for(QString s: data){
            if(client->write(s.toUtf8()) == -1){
                qDebug() << "writing to client failed!!\n";
            }
        }
    }
    data.clear();
}
*/
void TCPManager::flush(){
    for(QTcpSocket* client: clients)
        client->flush();
}

TCPManager &TCPManager::operator<<(QString* s){
    for(QTcpSocket* client: clients){
        client->write(s->toUtf8());
    }

    return *this;
}

TCPManager &TCPManager::operator<<(stuff *s){
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    stream << *s;

    for(QTcpSocket* client: clients){
        client->write(block);
    }

    return *this;
}
