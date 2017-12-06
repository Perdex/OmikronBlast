#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"
#include "message.h"

#include <QtDebug>

Engine::Engine(Canvas& c, TCPManager& t) : items(), tcp(t), canvas(c)
{
    QObject::connect(&tcp, &TCPManager::idReceived,
                     this, &Engine::setPlayer);
}

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
    Message *msg;
    while(!data->atEnd()) {
        data->startTransaction();

        msg = Message::create(data);

        switch (msg->type()) {
        case MessageType::STATUS: {
            StatusMessage sm = *(static_cast<StatusMessage*>(msg));
            processStatus(sm);
            break;
        }
        case MessageType::UPDATE: {
            UpdateMessage um = *(static_cast<UpdateMessage*>(msg));
            processUpdate(um, data);
            break;
        }
        default:
            break;
        }

        if(!data->commitTransaction()) break;
    }
    delete msg;
    canvas.center();
}

void Engine::processStatus(StatusMessage& msg)
{

}

void Engine::processUpdate(UpdateMessage &msg, QDataStream *stream)
{
    *stream >> items[msg.id()];
}
