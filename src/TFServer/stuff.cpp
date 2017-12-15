#include "stuff.h"
#include "map.h"
#include "projectile.h"
#include "player.h"
#include <QDataStream>
#include <QtDebug>

stuff::stuff(Stuff t, qint16 id, Map *map, MainWindow *main, QDataStream *s,
             int m_x, int m_y)
    : id(id),
      x(m_x),
      y(m_y),
      vx(0),
      vy(0),
      isDead(false),
      type(t),
      map(map),
      mainWindow(main),
      stream(s)
{}

stuff::~stuff(){
    if(stream){
        delete stream;
        stream = nullptr;
    }
}

qint16 stuff::getId() const
{
    return id;
}
bool stuff::getIsDead() const
{
    return isDead;
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
           << s.getHorizontalPos() << s.getVerticalPos() << s.getIsDead();

    switch (s.type) {
    case Stuff::PLAYER: {
        player *p = (player*)&s;
        stream << p->getName() << p->getJetpackStatus()
               << p->getAmmoLeft() << (int)(p->getFuelLeft()+0.5) << p->getScore();
        break;
    }
    case Stuff::PROJECTILE: {
        stream << ((projectile*)&s)->getAngle();
        break;
    }
    default:
        break;
    }

    return stream;
}
