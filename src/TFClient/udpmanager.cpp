#include "udpmanager.h"
#include "message.h"
#include <QNetworkDatagram>
#include <QtDebug>

UDPManager::UDPManager() : sock(this), sport(2000), cport(2001)
{

    QObject::connect(&sock,
                     static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                     this,
                     &UDPManager::onError);
}

UDPManager::~UDPManager() {}

void UDPManager::connectTo(const QString &hostname, QString name) {
    this->name = name;
    this->address = QHostAddress(hostname);

    sock.bind(QHostAddress::LocalHost, cport);

    qDebug() << "Sending handshake";
    QByteArray msg;
    QDataStream tmp(&msg, QIODevice::WriteOnly);
    tmp.setVersion(QDataStream::Qt_5_9);

    //TODO make this nicer
    tmp << (qint8)MessageType::STATUS << (qint8)CtoSStatus::HANDSHAKE << name;

    sock.writeDatagram(msg, address, sport);

    QObject::connect(&sock, &QUdpSocket::readyRead, this, &UDPManager::onReadyRead);
}


/*
 * This method is for pulling updates
 */
void UDPManager::onReadyRead() {
    qDebug() << "Received a datagram";

    // Set up the datastream for reading
    QByteArray data = sock.receiveDatagram().data();
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_5_9);

    emit updateReceived(&stream);
}

/*
 * This method is for pushing updates to server
 */
void UDPManager::onPushUpdate(const QMap<int, bool> &status, float ang, bool clicked) {
    QByteArray block;
    QDataStream tmp(&block, QIODevice::WriteOnly);
    tmp.setVersion(QDataStream::Qt_5_9);

    tmp << status << clicked << ang;

    sock.writeDatagram(block, address, sport);
}



void UDPManager::onError(QAbstractSocket::SocketError) {
    //qDebug() << err;
    emit error(sock.errorString());
}
