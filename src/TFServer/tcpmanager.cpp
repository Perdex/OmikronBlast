#include "tcpmanager.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>

TCPManager::TCPManager()
    : clients(),
      server(nullptr),
      port(2000),
      data()
{

    server = new QTcpServer[10];
    if (!server->listen()) {
        QMessageBox::critical(NULL, QString("TFServer"),
                              QString("Unable to start the server: %1.")
                              .arg(server->errorString()));
        server->close();
        return;
    }

    QObject::connect(server, &QTcpServer::newConnection, this, &TCPManager::newClient);

    /* esimerkistä kopioitu
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
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
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
quint16 TCPManager::getPort(){
    if(server != nullptr)
        return server->serverPort();
    return 0;
}

void TCPManager::newClient(){

    qDebug() << "ASDF client found!!1\n";

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

