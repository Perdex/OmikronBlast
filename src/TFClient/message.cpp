#include "message.h"
#include "stuff.h"

#include <QDataStream>
#include <QVariant>
#include <QtDebug>

Message* Message::create(QDataStream *stream)
{
    Message* msg = nullptr;
    stream->startTransaction();

    qint8 t;
    *stream >> t;
    MessageType mt = (MessageType)t;

    if(!stream->commitTransaction()) {
        delete msg;
        return nullptr;
    }

    switch (mt) {
    case MessageType::STATUS: {
        stream->startTransaction();

        qint8 gs;
        QVariant qv;
        *stream >> gs >> qv;

        if(!stream->commitTransaction()) {
            delete msg;
            return nullptr;
        }

        msg = (Message*)
                (new StatusMessage((GameStatus)gs, qv));
        break;
    }
    case MessageType::UPDATE: {
        stream->startTransaction();

        qint16 id;
        *stream >> id;

        if(!stream->commitTransaction()) {
            delete msg;
            return nullptr;
        }

        qDebug() << id;

        msg = (Message*)
                (new UpdateMessage(id));
        break;
    }
    default:
        break;
    }
    return msg;
}
