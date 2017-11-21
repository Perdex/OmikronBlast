#include "tcpmanager.h"
#include <QTcpSocket>
#include <QtDebug>

TCPManager::TCPManager() : sock(), port(-1), data()
{
    QObject::connect(&sock, &QTcpSocket::connected, this, &TCPManager::onConnected);
    QObject::connect(&sock, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);
    QObject::connect(&sock,
                     static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                     this,
                     &TCPManager::onError);
}

void TCPManager::connectTo(const QString &hostname, const int port) {
    this->port = port;
    sock.connectToHost(hostname, port);
}

void TCPManager::disconnect(const QString &reason) {
    sock.disconnect();
    emit disconnected(reason);
}

/*
 * This method is for pulling updates
 */
void TCPManager::onReadyRead() {
    emit updateReceived(data);
}

/*
 * This method is for pushing updates to server
 */
void TCPManager::onPushUpdate() {

}

/*
 * Initialize connection
 */
void TCPManager::onConnected() {

    /*data->setDevice(&sock);
    data->setVersion(QDataStream::Qt_5_9);

    sock.write("TFGAME-CLIENT");

    QString msg;
    do{
        data->startTransaction();
        *data >> msg;
    }while(!data->commitTransaction());

    if(msg != "TFGAME-SERVER") {
        disconnect("Bad message from server.");
        return;
    }

    char *id;
    do{
        data->startTransaction();
        *data >> id;
    }while(!data->commitTransaction());

    emit idReceived(*id);

    // TODO: Read map

    emit mapReceived(/*map);*/

    emit connected();

    QObject::connect(&sock, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
}

/*
 * Clean up after disconnect
 */
void TCPManager::onDisconnected() {

}

void TCPManager::onError(QAbstractSocket::SocketError err) {
    emit error(err);
}
