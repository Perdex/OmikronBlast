#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>
#include <QDataStream>
#include "stuff.h"

enum class MessageType : qint8 {
    UPDATE, STATUS
};

enum class GameStatus : qint8 {
    HANDSHAKE,
    ID_TRANSFER,
    MAP_TRANSFER,
    ROUND_END,
    START,
    PAUSED,
    UNPAUSED,
    END
};

class Message
{
    /*
     * This class is only used as base class for StatusMessage and
     * UpdateMessage, and thus it should never be constructed manually.
     */
public:
    Message() {}
    virtual ~Message() {}
    Message(MessageType t) : m_type(t) {}
    const MessageType& type() const { return m_type; }
    static Message* create(QDataStream*);
private:
    MessageType m_type;
};

class StatusMessage : public Message
{
public:
    StatusMessage(GameStatus s, QVariant d)
        : Message(MessageType::STATUS), m_status(s), m_data(d) {}
    GameStatus status() const { return m_status; }
    template <typename T> const T data() const { return m_data.value<T>(); }
private:
    GameStatus m_status;
    QVariant m_data;
};

class UpdateMessage : public Message
{
public:
    UpdateMessage(Stuff t, qint16 i)
        : Message(MessageType::UPDATE), m_id(i), m_type(t) {}
    qint16 id() const { return m_id; }
    Stuff datatype() const { return m_type; }
private:
    qint16 m_id;
    Stuff m_type;
};

#endif // MESSAGE_H
