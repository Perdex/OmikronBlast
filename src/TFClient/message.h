#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>
#include <QDataStream>
class stuff;

enum MessageType {
    UPDATE, STATUS
};

enum GameStatus {
    COUNTDOWN, START, PAUSED, UNPAUSED, END
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
    friend QDataStream& operator>>(QDataStream&, Message&);
private:
    MessageType m_type;
};

template<typename T>
class StatusMessage : public Message
{
public:
    StatusMessage(GameStatus s, QVariant& d)
        : Message(STATUS), m_status(s), m_data(d) {}
    const GameStatus status() const { return m_status; }
    const  T data() const { return m_data.value<T>(); }
    friend QDataStream& operator>>(QDataStream&, StatusMessage&);
private:
    GameStatus m_status;
    QVariant m_data;
};

class UpdateMessage : public Message
{
public:
    UpdateMessage(stuff* s)
        : Message(UPDATE), m_stuff(s) {}
    const stuff* data() const { return m_stuff; }
    friend QDataStream& operator>>(QDataStream&, UpdateMessage&);
private:
    stuff* m_stuff;
};

#endif // MESSAGE_H
