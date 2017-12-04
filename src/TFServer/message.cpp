#include "message.h"
#include "stuff.h"

QDataStream& operator<<(QDataStream& stream, Message* msg)
{
    stream << (qint8)msg->type();
    switch (msg->type()) {
    case MessageType::UPDATE: {
        UpdateMessage* um = static_cast<UpdateMessage*>(msg);
        stream << um;
        break;
    }
    case MessageType::STATUS: {
        StatusMessage* sm = static_cast<StatusMessage*>(msg);
        stream << sm;
        break;
    }
    default:
        break;
    }

    return stream;
}

QDataStream& operator<<(QDataStream& stream, StatusMessage* msg)
{
    stream << (qint8)msg->m_status << msg->m_data;
    return stream;
}

QDataStream& operator<<(QDataStream& stream, UpdateMessage* msg)
{
    stream << *(msg->m_stuff);
    return stream;
}
