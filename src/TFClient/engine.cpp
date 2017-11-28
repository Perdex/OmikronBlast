#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"

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
}

void Engine::setPlayer(qint16 id) {
    my_id = id;

    //TODO jonnekkin muualle.
    items[id] = new player("pertti", 'f', 2500, 2500);

    player *p = static_cast<player*>(items[id]);

    //TODO Jaakkooo!!!!!!
    //p->setPos(p->getHorizontalPos(), p->getVerticalPos());
    //DONED

    canvas.setMyPlayer(p);
    canvas.addPlayer(p);
}


void Engine::readData(QDataStream* data) {
    while(!data->atEnd()) {
        data->startTransaction();

        qint16 id;
        *data >> id;

        //items[id]->decode(data);

        if(!data->commitTransaction()) return;
    }
}
