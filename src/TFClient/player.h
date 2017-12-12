#ifndef PLAYER_H
#define PLAYER_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QtGui>

class player :public stuff
{
public:
    player(QString name, qint16 id, double x, double y);
    ~player();
    void update(QDataStream*);
    static player* create(qint16 id, QDataStream*);
    void setAngle(double angle);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QString name;
    bool isDead;
    double angle;
    int ammo;
    int fuel;
    bool isMe;
    bool jetpackActive;
    QPixmap pixmaps[3];
    QPixmap flame;
};

#endif // PLAYER_H
