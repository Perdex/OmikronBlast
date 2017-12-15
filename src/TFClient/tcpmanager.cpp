#include "tcpmanager.h"
#include <QTcpSocket>
#include <QtDebug>

TCPManager::TCPManager() : sock()
{
    QObject::connect(&sock, &QTcpSocket::connected, this, &TCPManager::onConnected);
    QObject::connect(&sock, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);
    QObject::connect(&sock,
                     static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                     this,
                     &TCPManager::onError);
}

TCPManager::~TCPManager() {
    delete data;
}

void TCPManager::connectTo(const QString &hostname, const int port, QString name) {
    this->name = name;
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
void TCPManager::onPushUpdate(const QMap<int, bool> &status, float ang, bool clicked) {
    QByteArray block;
    QDataStream tmp(&block, QIODevice::WriteOnly);
    tmp.setVersion(QDataStream::Qt_5_9);

    tmp << status << clicked << ang;

    sock.write(block);
    sock.flush();
}

/*
 * Initialize connection
 */
void TCPManager::onConnected() {

    data = new QDataStream(&sock);
    data->setVersion(QDataStream::Qt_5_9);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    out << QString("TFGAME-CLIENT") << name;
    sock.write(block);
    sock.flush();

    QObject::connect(&sock, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);

    emit connected();
}

/*
 * Clean up after disconnect
 */
void TCPManager::onDisconnected() {

}

void TCPManager::onError(QAbstractSocket::SocketError) {
    //qDebug() << err;
    emit error(sock.errorString());
}
