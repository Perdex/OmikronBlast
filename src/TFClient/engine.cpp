#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"

Engine::Engine(Canvas& c, TCPManager& t) : items(), tcp(t), canvas(c)
{

}

void Engine::readData(QDataStream* data) {

}
