#include "udpmanager.h"
#include "mainwindow.h"

#include <QUdpSocket>
#include <QTcpSocket>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QString>
#include <QVariant>

#include "message.h"
#include "stuff.h"

UDPManager::UDPManager(MainWindow* mainWindow)
    : sport(2000),
      cport(2001),
      mainWindow(mainWindow),
      addresses()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, sport);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}
UDPManager::~UDPManager()
{}

/*
 * Game is starting, stop accepting players (might be changed later)
 */
void UDPManager::gameStarted(){}

QString UDPManager::getAddress(){

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

void UDPManager::readData(){
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();

        // Set up datastream from datagram
        auto data = datagram.data();
        QDataStream in(&data, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_9);

        // Read the type of the message
        qint8 messageType;
        in >> messageType;

        if(messageType == (qint8)MessageType::UPDATE){
            mainWindow->processUpdate(in);
        }else if(messageType == (qint8)MessageType::STATUS){
            qDebug() << "UDPMgr: Datagram type: STATUS";
            processStatus(in, datagram);
        }else
            qDebug() << "UDPMgr: Unknown message type:" << messageType;


    }
}

/*
 * Process status messages
 */
void UDPManager::processStatus(QDataStream &stream, QNetworkDatagram &datagram){
    // Read status type
    qint8 type;
    stream >> type;
    if(type == (qint8)CtoSStatus::HANDSHAKE)
        processHandshake(stream, datagram);
    else
        qDebug() << "UDPMgr: check was" << type;
}

/*
 * Process the handshake from client
 * - Reads player name
 * - Writes id
 */
void UDPManager::processHandshake(QDataStream &stream, QNetworkDatagram &datagram)
{
    addresses += datagram.senderAddress();

    QString name;
    stream >> name;

    {
        // Set up outgoing stream
        QByteArray msg;
        QDataStream out(&msg, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_9);

        // Create a handshake message
        StatusMessage sm_hs = StatusMessage(StoCStatus::HANDSHAKE, QString("OMIKRON-SERVER"));
        out << &sm_hs;
        // Send the ID
        socket->writeDatagram(msg, datagram.senderAddress(), cport);
    }

    // Set up outgoing stream
    QByteArray msg;
    QDataStream out(&msg, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    // Create a new ID
    qint16 id = mainWindow->getNextId();
    qDebug() << "UDPMgr: id is" << id;
    StatusMessage sm_id = StatusMessage(StoCStatus::ID_TRANSFER, QVariant(id));
    out << &sm_id;
    // Send the ID
    socket->writeDatagram(msg, datagram.senderAddress(), cport);
    mainWindow->addPlayer(id, name);

}

UDPManager &UDPManager::operator<<(Message *msg) {
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    stream << msg;

    for(QHostAddress address: addresses){
        socket->writeDatagram(block, address, cport);
    }

    return *this;
}
