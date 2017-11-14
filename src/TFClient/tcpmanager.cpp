#include "tcpmanager.h"
#include <QTcpSocket>

TCPManager::TCPManager() : sock(), port(2000), data()
{
    QObject::connect(&sock, &QTcpSocket::connected, this, &TCPManager::onConnected);
    QObject::connect(&sock, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);

    data->setDevice(&sock);
    data->setVersion(QDataStream::Qt_5_9);
}

void TCPManager::connectTo(const QString &hostname) {
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

    emit mapReceived(/*map*/);

    QObject::connect(&sock, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
}

/*
 * Clean up after disconnect
 */
void TCPManager::onDisconnected() {

}
