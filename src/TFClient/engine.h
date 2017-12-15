#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>
#include <QObject>
#include <QSound>

class TCPManager;
class Canvas;
class Infobox;
class stuff;
class UpdateMessage;
class StatusMessage;

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(Canvas&, Infobox&, TCPManager&);
    ~Engine();
    void start();
signals:
    void started();
    void pauseChanged(bool);
    void ended();
private:
    void processUpdate(UpdateMessage*, QDataStream*);
    void processStatus(StatusMessage*);

    void addStuff(stuff*);

    QMap<qint16, stuff*> items;
    TCPManager& tcp;
    Canvas& canvas;
    Infobox& infobox;
    qint16 my_id;
    QSound theme;
private slots:
    //void setPlayer(qint16 id);
    void readData(QDataStream*);
};

#endif // ENGINE_H
