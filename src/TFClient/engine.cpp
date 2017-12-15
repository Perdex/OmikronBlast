#include "engine.h"

#include "tcpmanager.h"
#include "stuff.h"
#include "canvas.h"
#include "infobox.h"
#include "message.h"

#include <QtDebug>

Engine::Engine(Canvas& c, Infobox& i, TCPManager& t) : items(), tcp(t), canvas(c), infobox(i)
{}

Engine::~Engine() {

}

void Engine::start() {
    QObject::connect(&tcp, &TCPManager::updateReceived, this, &Engine::readData);
    QObject::connect(&canvas, &Canvas::statusChanged, &tcp, &TCPManager::onPushUpdate);
}

void Engine::addStuff(stuff* s) {
    items.insert(s->getId(), s);
    if(s->getId() == my_id)
    {
        canvas.setMyPlayer(static_cast<player*>(s));
        infobox.setMyPlayer(static_cast<player*>(s));
    }
    if(s->getType() == Stuff::PLAYER)
    {
        infobox.addPlayer(static_cast<player*>(s));
    }
    canvas.addStuff(s);
}


void Engine::readData(QDataStream* data) {
    while(!data->atEnd()) {
        data->startTransaction();

        Message *msg = Message::create(data);

        if(!data->commitTransaction()) break;

        //qDebug() << "Type" << (qint8)msg->type();

        switch (msg->type()) {
        case MessageType::STATUS: {
            StatusMessage *sm = static_cast<StatusMessage*>(msg);
            processStatus(sm);
            break;
        }
        case MessageType::UPDATE: {
            UpdateMessage *um = static_cast<UpdateMessage*>(msg);
            processUpdate(um, data);
            break;
        }
        default:
            break;
        }
        delete msg;
    }
    canvas.center();
    infobox.update();
}

void Engine::processStatus(StatusMessage* msg)
{
    //qDebug() << "Status" << (qint8)msg->status();
    switch (msg->status()) {
    case GameStatus::HANDSHAKE: {
        if(msg->data<QString>() != "TFGAME-SERVER") {
            qDebug() << "Smthing is amiss.";
            tcp.disconnect("Bad server type.");
        }
        break;
    }
    case GameStatus::ID_TRANSFER: {
        qint16 id = msg->data<qint16>();
        my_id = id;
        break;
    }
    case GameStatus::MAP_TRANSFER: {
        canvas.buildMap(msg->data<QString>());
        break;
    }
    case GameStatus::ROUND_END: {
        //TODO add round end message
        for(stuff *s: items)
            delete s;
        items.clear();
        break;
    }
    case GameStatus::START: {
        //TODO add 3-second counter to screen
        emit started();
        canvas.center();
        infobox.countDown(3);
        break;
    }
    case GameStatus::PAUSED: {
        //TODO add pause message
        infobox.countDown(-1);
        break;
    }
    case GameStatus::END: {
        //TODO add end message
        infobox.countDown(-2);
        break;
    }
    default:
        break;
    }
}

void Engine::processUpdate(UpdateMessage *msg, QDataStream *stream)
{
    if(!items.contains(msg->id())) {
        addStuff(stuff::create(msg, stream));
    }else{
        stuff *s = items[msg->id()];
        s->update(stream);

        /*
        if(s->getType() == Stuff::PLAYER && ((player*)s)->dead()
                && msg->id() != my_id) {
            items.remove(msg->id());
            canvas.remove(s);
        }
        */
    }
}
