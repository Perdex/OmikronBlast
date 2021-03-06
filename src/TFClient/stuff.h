#ifndef STUFF_H
#define STUFF_H
#include <QString>

#include <QObject>
#include <QGraphicsItem>
#include <QVector>

class UpdateMessage;

enum class Stuff : qint8 {
    PLAYER, PROJECTILE
};

class stuff: public QGraphicsItem
{

public:
    stuff(Stuff t, qint16 id, double &x, double &y);
    ~stuff();
    double getHorizontalPos();
    double getVerticalPos();
    qint16 getId() const {return id;}
    void setHorizontalPos(double);
    void setVerticalPos(double);
    virtual void update(QDataStream*) = 0;

    friend QDataStream& operator>>(QDataStream&, stuff*);
    static stuff* create(UpdateMessage*, QDataStream*);
    bool getIsDead();
    void setIsDead(bool s);

    Stuff getType() const { return type; }
protected:
    Stuff type;
    qint16 id;
    double horizontalPos;
    double verticalPos;
    bool isDead;
};
#endif // STUFF_H
