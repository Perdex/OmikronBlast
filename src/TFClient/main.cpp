#include "frame.h"
#include "tcpmanager.h"
#include <QApplication>
#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Frame w;
    TCPManager tcp;
    Engine eng(w.getCanvas(), tcp);

    QObject::connect(&tcp, &TCPManager::connected, &w, &Frame::onConnected);
    QObject::connect(&tcp, &TCPManager::connected, &eng, &Engine::start);
    QObject::connect(&w, &Frame::requestConnection, &tcp, &TCPManager::connectTo);

    w.show();

    return a.exec();
}
