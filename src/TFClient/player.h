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
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QString name;
    bool isDead;
    double angle;
    QChar ammo;
    QChar fuel;
    bool isMe;
    QChar mouseClicked;
};

#endif // PLAYER_H
