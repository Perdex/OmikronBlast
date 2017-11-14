#include "frame.h"
#include "tcpmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Frame w;
    TCPManager tcp;

    QObject::connect(&tcp, &TCPManager::connected, &w, &Frame::onConnected);

    w.show();

    return a.exec();
}
