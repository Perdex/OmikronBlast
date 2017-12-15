#ifndef PLAYER_H
#define PLAYER_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QtGui>
#include <QSound>

class player :public stuff
{
public:
    player(QString name, qint16 id, double x, double y, int score);
    ~player();
    void update(QDataStream*);
    static player* create(qint16 id, QDataStream*);
    void setAngle(double angle);
    int getAmmo();
    int getFuel();
    int getScore();
    QString getName();
    bool getDead() {return isDead;}
    bool operator<(player*);
    bool isOnGround() { return onGround; }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QString name;
    double angle;
    int ammo;
    int fuel;
    int score;
    bool jetpackActive;
    QPixmap marine;
    QPixmap gun;
    QPixmap flame;
    QPixmap stone;
    bool onGround;
    QSound jetpack;
};

#endif // PLAYER_H
