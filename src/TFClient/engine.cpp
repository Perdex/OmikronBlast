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
    int i=1800;
    while(i<2800){
        if(i==1800 || i==2700){
            int j=1900;
                    while(j<2900){
                    Item* item= new Item();
                    canvas.addItem(item, i, j);
                    j=j+100;
                    }
             i=i+100;
        }
        else{
            Item* item= new Item();
            canvas.addItem(item, i, 1800);
            Item* item2= new Item();
            canvas.addItem(item2, i, 2700);
            i=i+100;
        }
    }

    canvas.setMyPlayer(p);
    canvas.addPlayer(p);
}


void Engine::readData(QDataStream* data) {
    //qDebug() << items.values();
    while(!data->atEnd()) {
        data->startTransaction();

        qint16 id;
        *data >> id;

        *data >> items[id];

        if(!data->commitTransaction()) return;
    }
    canvas.center();
}
