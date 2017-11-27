#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"

Engine::Engine(const Canvas& c, TCPManager& t) : items(), tcp(t), canvas(c)
{
}

Engine::~Engine() {

}

void Engine::start() {
    QObject::connect(&tcp, &TCPManager::updateReceived, this, &Engine::readData);
}

void Engine::readData(QDataStream* data) {
    while(!data->atEnd()) {
        data->startTransaction();

        qint16 id;
        *data >> id;

        //items[*id]->encode(data);

        if(!data->commitTransaction()) return;
    }
}
