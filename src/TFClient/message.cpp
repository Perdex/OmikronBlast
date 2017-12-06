#include "message.h"
#include "stuff.h"

#include <QDataStream>
#include <QVariant>

Message* Message::create(QDataStream *stream)
{
    Message* msg;
    stream->startTransaction();

    qint8 t;
    *stream >> t;
    MessageType mt = (MessageType)t;

    switch (mt) {
    case MessageType::STATUS: {
        qint8 gs;
        QVariant qv;
        *stream >> gs >> qv;
        msg = (Message*)
                (new StatusMessage((GameStatus)gs, qv));
        break;
    }
    case MessageType::UPDATE: {
        qint16 id;
        *stream >> id;
        msg = (Message*)
                (new UpdateMessage(id));
        break;
    }
    default:
        break;
    }

    if(!stream->commitTransaction()) {
        delete msg;
        return NULL;
    }
    return msg;
}
