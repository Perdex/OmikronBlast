#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>

#include "tcpmanager.h"
#include "canvas.h"
#include "stuff.h"

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(const Canvas&, TCPManager&);
    ~Engine();
    void start();
private:
    QMap<qint16, stuff*> items;
    TCPManager& tcp;
    const Canvas& canvas;
private slots:
    void readData(QDataStream*);
};

#endif // ENGINE_H
