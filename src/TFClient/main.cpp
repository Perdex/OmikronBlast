#include "frame.h"
#include "tcpmanager.h"
#include <QApplication>
#include "engine.h"

#include <QSoundEffect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Frame w;
    TCPManager tcp;
    Engine eng(w.getCanvas(), w.getBox(), tcp);

    QObject::connect(&tcp, &TCPManager::connected, &w, &Frame::onConnected);
    QObject::connect(&tcp, &TCPManager::connected, &eng, &Engine::start);
    QObject::connect(&w, &Frame::requestConnection, &tcp, &TCPManager::connectTo);
    QObject::connect(&tcp, &TCPManager::error, &w, &Frame::setStatus);
    QObject::connect(&eng, &Engine::started, &w, &Frame::showGame);

    w.show();

    return a.exec();
}
