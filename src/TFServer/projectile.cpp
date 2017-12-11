#include "projectile.h"
#include <cmath>

#define SPEED 20


projectile::projectile(qint16 id, double& x, double& y, player& owner, double angle)
    : stuff(id,x,y,0,0), owner(owner)
{
    setVerticalSpeed(sin(angle) * SPEED);
    setHorizontalSpeed(cos(angle) * SPEED);
}
projectile::~projectile(){}

void projectile::move()
{
    changeVerticalPos(getVerticalSpeed());
    changeHorizontalPos(getHorizontalPos());
}
void projectile::hitPlayer(player& victim)
{
    victim.die();
    if(victim.getId() != owner.getId())
    {
        owner.getPoint();
    }
}

