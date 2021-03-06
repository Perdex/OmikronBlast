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
    server->pauseAccepting();
}

QString TCPManager::getAddress(){
    if(!server)
        return QString("Error when setting up server");

    QString ipAddress;

    //Use a dns lookup for determining IP address
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS
    if (socket.waitForConnected()) {
        ipAddress = socket.localAddress().toString();
    } else {
        qWarning()
            << "could not determine local IPv4 address:"
            << socket.errorString();
    }
    socket.close();
    return ipAddress;
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
    in->setVersion(QDataStream::Qt_5_0);

    QString cver;
    QString name;
    do{
        in->startTransaction();
        *in >> cver >> name;
    }while(!in->commitTransaction());

    if(cver != "TFGAME-CLIENT") {
        qDebug() << cver;
        return;
    }

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    QVariant qhs("TFGAME-SERVER");
    StatusMessage sm_hs = StatusMessage(GameStatus::HANDSHAKE, qhs);

    qint16 id = mainWindow->getNextId();
    QVariant qid(id);
    StatusMessage sm_id = StatusMessage(GameStatus::ID_TRANSFER, qid);

    stream << &sm_hs << &sm_id;

    qDebug() << msg;

    socket->write(msg);
    socket->flush();

    mainWindow->addPlayer(in, id, name);
}

void TCPManager::clientLost(){
    qDebug() << "A client was lost!";
    //mainWindow->endGame();
}

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
    stream.setVersion(QDataStream::Qt_5_0);
    stream << *s;

    for(QTcpSocket* client: clients){
        client->write(block);
    }

    return *this;
}

TCPManager &TCPManager::operator<<(Message *msg) {
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << msg;

    for(QTcpSocket* client: clients){
        client->write(block);
    }

    return *this;
}
