#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>
#include <QObject>
#include <QSound>

class UDPManager;
class Canvas;
class Infobox;
class stuff;
class UpdateMessage;
class StatusMessage;

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(Canvas&, Infobox&, UDPManager&);
    ~Engine();
    void start();
signals:
    void started();
    void pauseChanged(bool);
    void ended();
    void connected();
private:
    void processUpdate(UpdateMessage*, QDataStream*);
    void processStatus(StatusMessage*);

    void addStuff(stuff*);

    QMap<qint16, stuff*> items;
    UDPManager& udp;
    Canvas& canvas;
    Infobox& infobox;
    qint16 my_id;
    QSound theme;
public slots:
    //void setPlayer(qint16 id);
    void readData(QDataStream*);
};

#endif // ENGINE_H
