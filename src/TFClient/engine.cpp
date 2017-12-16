#include "engine.h"

#include "udpmanager.h"
#include "stuff.h"
#include "canvas.h"
#include "infobox.h"
#include "message.h"

#include <QtDebug>
#include <QSound>
#include <QFile>

Engine::Engine(Canvas& c, Infobox& i, UDPManager& u) : items(), udp(u), canvas(c), infobox(i),  theme(":/sounds/Sounds/theme.wav")
{
    theme.setLoops(QSound::Infinite);
}

Engine::~Engine() {}

void Engine::start() {
    QObject::connect(&canvas, &Canvas::statusChanged, &udp, &UDPManager::onPushUpdate);
}

void Engine::addStuff(stuff* s) {
    items.insert(s->getId(), s);
    if(s->getId() == my_id)
    {
        canvas.setMyPlayer(static_cast<player*>(s));
        infobox.setMyPlayer(static_cast<player*>(s));
    }
    canvas.addStuff(s);
}


void Engine::readData(QDataStream* data) {
    while(!data->atEnd()) {
        data->startTransaction();

        qDebug() << "Building a message";
        Message *msg = Message::create(data);

        if(!data->commitTransaction()) break;

        //qDebug() << "Type" << (qint8)msg->type();

        switch (msg->type()) {
        case MessageType::STATUS: {
            qDebug() << "Message type was STATUS";
            StatusMessage *sm = static_cast<StatusMessage*>(msg);
            processStatus(sm);
            break;
        }
        case MessageType::UPDATE: {
            qDebug() << "Message type was UPDATE";
            UpdateMessage *um = static_cast<UpdateMessage*>(msg);
            processUpdate(um, data);
            break;
        }
        default:
            qDebug() << "Message type was unknown!";
            break;
        }
        delete msg;
    }
    canvas.center();
    infobox.update(items);
}

void Engine::processStatus(StatusMessage* msg)
{
    switch (msg->status()) {
    case StoCStatus::HANDSHAKE: {
        if(msg->data<QString>() != "TFGAME-SERVER") {
            qDebug() << "Smthing is amiss.";
        }
        emit connected();
        break;
    }
    case StoCStatus::ID_TRANSFER: {
        qint16 id = msg->data<qint16>();
        my_id = id;
        qDebug() << "ID_TRANSFER received: my id set to" << id;
        break;
    }
    case StoCStatus::MAP_TRANSFER: {
        canvas.buildMap(msg->data<QString>());
        break;
    }
    case StoCStatus::ROUND_END: {
        for(stuff *s: items) {
            delete s;
            s = nullptr;
        }
        canvas.setMyPlayer(nullptr);
        infobox.setMyPlayer(nullptr);
        items.clear();
        break;
    }
    case StoCStatus::START: {
        emit started();
        canvas.center();
        infobox.countDown(3);

        theme.play();

        break;
    }
    case StoCStatus::PAUSED: {
        infobox.countDown(-1);
        break;
    }
    case StoCStatus::END: {
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

        if(items[msg->id()]->getType() == Stuff::PROJECTILE) {
            QSound::play(":/sounds/Sounds/shoot.wav");
        }
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
