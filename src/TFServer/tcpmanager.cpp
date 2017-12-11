#include "tcpmanager.h"
#include "mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QString>
#include <QVariant>

#include "message.h"
#include "stuff.h"

TCPManager::TCPManager(MainWindow* mainWindow)
    : clients(),
      server(nullptr),
      port(2000),
      mainWindow(mainWindow)
{

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, port)) {
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

/*
 * Game is starting, stop accepting players (might be changed later)
 */
void TCPManager::gameStarted(){
    //server->pauseAccepting();
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

    if(!socket->waitForReadyRead(3000)) {
        qDebug() << "Connection timed out.";
        return;
    }

    clients.push_back(socket);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &TCPManager::clientLost);

    qDebug() << "Client found!";

    //This is deleted in destructor of stuff
    QDataStream *in = new QDataStream(socket);
    in->setVersion(QDataStream::Qt_5_9);

    QString cver;
    do{
        in->startTransaction();
        *in >> cver;
    }while(!in->commitTransaction());

    if(cver != "TFGAME-CLIENT") {
        qDebug() << cver;
        return;
    }

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);

    QVariant qhs("TFGAME-SERVER");
    StatusMessage sm_hs = StatusMessage(GameStatus::HANDSHAKE, qhs);

    qint16 id = clients.size();
    QVariant qid(id);
    StatusMessage sm_id = StatusMessage(GameStatus::ID_TRANSFER, qid);

    stream << &sm_hs << &sm_id;

    qDebug() << msg;

    socket->write(msg);
    socket->flush();

    mainWindow->addPlayer(id, in);
}

void TCPManager::clientLost(){
    qDebug() << "A client was lost!";
    //mainWindow->endGame();
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
    qDebug() << "Deprecation warning: TCPManager::operator<<(QString* s)";
    for(QTcpSocket* client: clients){
        client->write(s->toUtf8());
    }

    return *this;
}

TCPManager &TCPManager::operator<<(stuff *s){
    qDebug() << "Deprecation warning: TCPManager::operator<<(stuff* s)";
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    stream << *s;

    for(QTcpSocket* client: clients){
        client->write(block);
    }

    return *this;
}

TCPManager &TCPManager::operator<<(Message *msg) {
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    stream << msg;

    for(QTcpSocket* client: clients){
        client->write(block);
    }

    return *this;
}
