#include "projectile.h"
#include "map.h"
#include "message.h"
#include "tcpmanager.h"
#include <cmath>

#define SPEED 1
#define RADIUS 10


projectile::projectile(qint16 id, double x, double y, player *owner, double angle,
                       Map *map, MainWindow *main)
    : stuff(Stuff::PROJECTILE, id, map, main, nullptr, x,y), owner(owner)
{
    qDebug() << angle;
    setVerticalSpeed(sin(angle) * SPEED);
    setHorizontalSpeed(cos(angle) * SPEED);
    bounceCount = 0;
}
projectile::~projectile(){}

void projectile::doStep(int dt){

}
void projectile::move(int dt, TCPManager &mgr){

    bool test;

    test = map->collide(&x, &y, &vx, &vy, dt, 0.9);
    x += dt * vx;
    y += dt * vy;

    if(test)
        bounceCount += 1;
    UpdateMessage msg(this);
    mgr << &msg;
}

void projectile::hitPlayer(player& victim)
{
    victim.die();
    if(victim.getId() != owner->getId())
    {
        owner->getPoint();
    }
}

