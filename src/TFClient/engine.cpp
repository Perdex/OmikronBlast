#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"
#include "message.h"

#include <QtDebug>

Engine::Engine(Canvas& c, TCPManager& t) : items(), tcp(t), canvas(c)
{}

Engine::~Engine() {

}

void Engine::start() {
    QObject::connect(&tcp, &TCPManager::updateReceived, this, &Engine::readData);
    QObject::connect(&canvas, &Canvas::statusChanged, &tcp, &TCPManager::onPushUpdate);
}

void Engine::setPlayer(qint16 id) {
    my_id = id;

    //TODO jonnekkin muualle.
    items[id] = new player("pertti", 'f', 2500, 2500);

    player *p = static_cast<player*>(items[id]);

    //TODO Jaakkooo!!!!!!
    //p->setPos(p->getHorizontalPos(), p->getVerticalPos());
    //DONED


    //canvas.buildMap(p[39][39]);

    canvas.setMyPlayer(p);
    canvas.addPlayer(p);
}


void Engine::readData(QDataStream* data) {
    while(!data->atEnd()) {
        data->startTransaction();

        Message *msg = Message::create(data);

        if(!data->commitTransaction()) break;

        qDebug() << "Type" << (qint8)msg->type();

        switch (msg->type()) {
        case MessageType::STATUS: {
            StatusMessage *sm = static_cast<StatusMessage*>(msg);
            processStatus(sm);
            break;
        }
        case MessageType::UPDATE: {
            UpdateMessage *um = static_cast<UpdateMessage*>(msg);
            processUpdate(um, data);
            break;
        }
        default:
            break;
        }
        delete msg;
    }
    canvas.center();
}

void Engine::processStatus(StatusMessage* msg)
{
    qDebug() << "Status" << (qint8)msg->status();
    switch (msg->status()) {
    case GameStatus::HANDSHAKE: {
        if(msg->data<QString>() != "TFGAME-SERVER") {
            qDebug() << "Smthing is amiss.";
            tcp.disconnect("Bad server type.");
        }
        break;
    }
    case GameStatus::ID_TRANSFER: {
        qint16 id = msg->data<qint16>();
        setPlayer(id);
        break;
    }
    case GameStatus::MAP_TRANSFER: {

        break;
    }
    case GameStatus::COUNTDOWN: {
        break;
    }
    case GameStatus::START: {
        emit started();
        canvas.center();
        break;
    }
    case GameStatus::PAUSED: {
        break;
    }
    case GameStatus::END: {
        break;
    }
    default:
        break;
    }
}

void Engine::processUpdate(UpdateMessage *msg, QDataStream *stream)
{
    //qDebug() << msg->id() << items;
    stream->startTransaction();

    *stream >> items[msg->id()];

    stream->commitTransaction();
}
