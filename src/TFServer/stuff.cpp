#include "stuff.h"

stuff::stuff(qint16 id, double& x, double& y, double dx = 0 , double dy = 0) : id(id), horizontalPos(x), verticalPos(y),
             horizontalSpeed(dx), verticalSpeed(dy){}
stuff::~stuff(){}

qint16 stuff::getId() const
{
    return id;
}

double stuff::getVerticalPos() const
{
    return verticalPos;
}
double stuff::getHorizontalPos() const
{
    return horizontalPos;
}
double stuff::getVerticalSpeed() const
{
    return verticalSpeed;
}
double stuff::getHorizontalSpeed() const
{
    return horizontalSpeed;
}
void stuff::changeVerticalSpeed(double change)
{
    verticalSpeed += change;
}
void stuff::changeHorizontalSpeed(double change)
{
    horizontalSpeed += change;
}
void stuff::setVerticalSpeed(double speed)
{
    verticalSpeed = speed;
}
void stuff::setHorizontalSpeed(double speed)
{
    horizontalSpeed = speed;
}
void stuff::changeHorizontalPos(double change)
{
    horizontalPos += change;
}
void stuff::changeVerticalPos(double change)
{
    verticalPos += change;
}

QDataStream& operator<<(QDataStream& stream, stuff *s)
{
    stream << s->getId()
           << s->getHorizontalPos() << s->getVerticalPos();
    //TODO player(+projectile)-spesifiä juttua
    return stream;
}
