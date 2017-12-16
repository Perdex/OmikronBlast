#include "frame.h"
#include "udpmanager.h"
#include <QApplication>
#include "engine.h"

#include <QSoundEffect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Frame w;
    UDPManager udp;
    Engine eng(w.getCanvas(), w.getBox(), udp);

    QObject::connect(&eng, &Engine::connected, &w, &Frame::onConnected);
    QObject::connect(&eng, &Engine::connected, &eng, &Engine::start);
    QObject::connect(&w, &Frame::requestConnection, &udp, &UDPManager::connectTo);
    QObject::connect(&udp, &UDPManager::error, &w, &Frame::setStatus);
    QObject::connect(&eng, &Engine::started, &w, &Frame::showGame);
    QObject::connect(&udp, &UDPManager::updateReceived, &eng, &Engine::readData);

    w.show();

    return a.exec();
}
