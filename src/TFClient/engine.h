#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>

#include "tcpmanager.h"
#include "canvas.h"
#include "stuff.h"

class Engine
{
public:
    Engine(Canvas&, TCPManager&);
    ~Engine();
private:
    QMap<qint16, stuff*> items;
    TCPManager& tcp;
    Canvas& canvas;
private slots:
    void readData(QDataStream*);
};

#endif // ENGINE_H
