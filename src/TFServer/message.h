#ifndef MSG_H
#define MSG_H

#include <QVariant>
#include <QDataStream>
#include <QtDebug>

class stuff;

enum class MessageType {
    UPDATE, STATUS
};

enum class StoCStatus : qint8 {
    HANDSHAKE,
    ID_TRANSFER,
    MAP_TRANSFER,
    ROUND_END,
    START,
    PAUSED,
    UNPAUSED,
    END
};
enum class CtoSStatus : qint8 {
    HANDSHAKE
};

class Message
{
    /*
     * This class is only used as base class for StatusMessage and
     * UpdateMessage, and it should never be constructed manually.
     */
public:
    Message() {}
    virtual ~Message() {}
    Message(MessageType t) : m_type(t) {}
    const MessageType& type() const { return m_type; }
    friend QDataStream& operator<<(QDataStream&, Message*);
private:
    MessageType m_type;
protected:
    //virtual void toStream(QDataStream&) const = 0;
};

class StatusMessage : public Message
{
public:
    StatusMessage(StoCStatus s)
        : Message(MessageType::STATUS), m_status(s), m_data() {}
    StatusMessage(StoCStatus s, const QVariant& d)
        : Message(MessageType::STATUS), m_status(s), m_data(d) {}
    StoCStatus status() const { return m_status; }
    template<typename T> const T data() const { return m_data.value<T>(); }
    friend QDataStream& operator<<(QDataStream&, StatusMessage*);
private:
    StoCStatus m_status;
    QVariant m_data;
};

class UpdateMessage : public Message
{
public:
    UpdateMessage(stuff* s)
        : Message(MessageType::UPDATE), m_stuff(s) {}
    const stuff* data() const { return m_stuff; }
    friend QDataStream& operator<<(QDataStream&, UpdateMessage*);
private:
    stuff* m_stuff;
};

#endif // MSG_H
