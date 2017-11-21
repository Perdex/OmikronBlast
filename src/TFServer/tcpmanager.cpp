#include "tcpmanager.h"
#include "mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>

TCPManager::TCPManager(MainWindow* mainWindow)
    : clients(),
      server(nullptr),
      port(2000),
      data(),
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

    //QString s;
    //for(auto client: clients){
    //    s += client->peerName();
    //    s += '\n';
    //}
    mainWindow->setPlayersText(QString("Number connected: %1").arg(clients.length()));

}

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

void TCPManager::addData(QString s){
    data += s;
}

