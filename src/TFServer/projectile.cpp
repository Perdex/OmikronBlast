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
    setVerticalSpeed(sin(angle) * SPEED);
    setHorizontalSpeed(cos(angle) * SPEED);
}
projectile::~projectile(){}

void projectile::doStep(int dt){

}
void projectile::move(int dt, TCPManager &mgr){

    map->collide(&x, &y, &vx, &vy, dt, 1);
    x += dt * vx;
    y += dt * vy;

    mgr << new UpdateMessage(this);
}

void projectile::hitPlayer(player& victim)
{
    victim.die();
    if(victim.getId() != owner->getId())
    {
        owner->getPoint();
    }
}

