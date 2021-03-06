#include "projectile.h"
#include "map.h"
#include "message.h"
#include "tcpmanager.h"
#include "mainwindow.h"
#include <cmath>
#include <QTimer>

#define SPEED 1.5
#define RADIUS 10
#define MAX_BOUNCES 6
#define ACTIVATION_TIMEOUT 75

projectile::projectile(qint16 id, double x, double y, player *owner, double angle,
                       Map *map, MainWindow *main)
    : stuff(Stuff::PROJECTILE, id, map, main, nullptr, x,y),
      owner(owner),
      angle(angle)
{
    QTimer::singleShot(ACTIVATION_TIMEOUT, this, &projectile::activate);
    vx = cos(angle) * SPEED + owner->getHorizontalSpeed();
    vy = sin(angle) * SPEED + owner->getVerticalSpeed();
    this->x += cos(angle) * 10;
    this->y += sin(angle) * 10;
    bounceCount = 0;
}
projectile::~projectile(){}

bool projectile::doStep(){
    if(!isActive) return false;
    const int playerHitboxW = 30, playerHitboxH = 70;
    for(player *p: mainWindow->getPlayers().values()) {
        QRectF self(x - RADIUS, y - RADIUS, 2*RADIUS, 2*RADIUS);
        QRectF pl(p->getHorizontalPos() - playerHitboxW/2, p->getVerticalPos() - playerHitboxH/2, playerHitboxW, playerHitboxH);

        if(self.intersects(pl)) {
            hitPlayer(*p);
            return isDead = true;
        }
    }

    return false;
}
void projectile::move(int dt, TCPManager &mgr){

    bool test;

    if(!isDead)
    {
        //Collide with map
        test = map->collide(&x, &y, &vx, &vy, dt, 0.9);

        //Move
        x += dt * vx;
        y += dt * vy;

        if(test)
            bounceCount += 1;

        if(bounceCount > MAX_BOUNCES)
            isDead = true;
        else
            angle = 180. / M_PI * atan2(vy, vx);
    }


    UpdateMessage msg(this);
    mgr << &msg;
    //return angle;
}

void projectile::hitPlayer(player& victim)
{
    victim.die();
    if(victim.getId() != owner->getId())
    {
        owner->getPoint();
    }

}

