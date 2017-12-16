#include "message.h"
#include "stuff.h"

#include <QDataStream>
#include <QVariant>
#include <QtDebug>

QDataStream& operator<<(QDataStream& stream, StatusMessage* msg)
{
    stream << (qint8)msg->type() << (qint8)msg->m_status << msg->m_data;
    return stream;
}

Message* Message::create(QDataStream *stream)
{
    Message* msg = nullptr;
    stream->startTransaction();

    qint8 t;
    *stream >> t;
    MessageType mt = (MessageType)t;

    if(!stream->commitTransaction()) {
        qDebug() << "Message: couldn't read message type";
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
            qDebug() << "Message: couldn't read status details";
            delete msg;
            return nullptr;
        }

        qDebug() << "Status message contents are" << qv;

        msg = (Message*)
                (new StatusMessage((StoCStatus)gs, qv));
        break;
    }
    case MessageType::UPDATE: {
        stream->startTransaction();

        qint8 t;
        qint16 id;
        *stream >> t >> id;

        if(!stream->commitTransaction()) {
            qDebug() << "Message: couldn't read update details";
            delete msg;
            return nullptr;
        }

        //qDebug() << t << id;

        msg = (Message*)
                (new UpdateMessage((Stuff)t, id));
        break;
    }
    default:
        qDebug() << "Message: message type unknown" << (qint8)mt;
        break;
    }
    return msg;
}
