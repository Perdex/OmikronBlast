#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>
#include <QDataStream>
class stuff;

enum class MessageType : qint8 {
    UPDATE, STATUS
};

enum class GameStatus : qint8 {
    ID_TRANSFER,
    MAP_TRANSFER,
    COUNTDOWN,
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
    const GameStatus status() const { return m_status; }
    template <typename T> const T data() const { return m_data.value<T>(); }
private:
    GameStatus m_status;
    QVariant m_data;
};

class UpdateMessage : public Message
{
public:
    UpdateMessage(qint16 i)
        : Message(MessageType::UPDATE), m_id(i) {}
    const qint16 id() const { return m_id; }
private:
    qint16 m_id;
};

#endif // MESSAGE_H
