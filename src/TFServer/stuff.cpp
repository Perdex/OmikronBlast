#include "stuff.h"
#include "map.h"
#include <QDataStream>
#include <QtDebug>

stuff::stuff(qint16 id, double& x, double& y, double dx = 0 , double dy = 0)
    : id(id), x(x), y(y),
             vx(dx), vy(dy){}
stuff::stuff(Stuff t, qint16 id, QDataStream *s, Map *map) : id(id), stream(s), map(map) {
    //TODO
    x = 2500;
    y = 2500;
    vx = 0;
    vy = 0;
    type = t;
}

stuff::~stuff(){}

qint16 stuff::getId() const
{
    return id;
}

double stuff::getVerticalPos() const
{
    return y;
}
double stuff::getHorizontalPos() const
{
    return x;
}
double stuff::getVerticalSpeed() const
{
    return vy;
}
double stuff::getHorizontalSpeed() const
{
    return vx;
}
void stuff::changeVerticalSpeed(double change)
{
    vy += change;
}
void stuff::changeHorizontalSpeed(double change)
{
    vx += change;
}
void stuff::setVerticalSpeed(double speed)
{
    vy = speed;
}
void stuff::setHorizontalSpeed(double speed)
{
    vx = speed;
}
void stuff::changeHorizontalPos(double change)
{
    x += change;
}
void stuff::changeVerticalPos(double change)
{
    y += change;
}
void stuff::setHorizontalPos(double x)
{
    this->x = x;
}
void stuff::setVerticalPos(double y)
{
    this->y = y;
}

QDataStream& operator<<(QDataStream& stream, const stuff &s)
{
    stream << (qint8)s.type << s.getId()
           << s.getHorizontalPos() << s.getVerticalPos();
    //TODO player(+projectile)-spesifiä juttua
    return stream;
}
