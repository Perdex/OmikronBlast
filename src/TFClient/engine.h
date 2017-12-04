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
    Engine(Canvas&, TCPManager&);
    ~Engine();
    void start();
private:
    void processUpdate(QDataStream*);
    void processStatus(QDataStream*);

    QMap<qint16, stuff*> items;
    TCPManager& tcp;
    Canvas& canvas;
    qint16 my_id;
private slots:
    void setPlayer(qint16 id);
    void readData(QDataStream*);
};

#endif // ENGINE_H
