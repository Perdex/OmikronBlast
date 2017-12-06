#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>
#include <QObject>

class TCPManager;
class Canvas;
class stuff;
class UpdateMessage;
class StatusMessage;

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(Canvas&, TCPManager&);
    ~Engine();
    void start();
private:
    void processUpdate(UpdateMessage&, QDataStream*);
    void processStatus(StatusMessage&);

    QMap<qint16, stuff*> items;
    TCPManager& tcp;
    Canvas& canvas;
    qint16 my_id;
private slots:
    void setPlayer(qint16 id);
    void readData(QDataStream*);
};

#endif // ENGINE_H
