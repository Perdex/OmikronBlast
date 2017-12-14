#include "projectile.h"
#include "map.h"
#include "message.h"
#include "tcpmanager.h"
#include "mainwindow.h"
#include <cmath>
#include <QTimer>

#define SPEED 1
#define RADIUS 10
#define MAX_BOUNCES 10

projectile::projectile(qint16 id, double x, double y, player *owner, double angle,
                       Map *map, MainWindow *main)
    : stuff(Stuff::PROJECTILE, id, map, main, nullptr, x,y),
      owner(owner),
      angle(angle)
{
    QTimer::singleShot(100, this, &projectile::activate);
    setVerticalSpeed(sin(angle) * SPEED);
    setHorizontalSpeed(cos(angle) * SPEED);
    bounceCount = 0;
}
projectile::~projectile(){}

bool projectile::doStep(int dt){
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

    test = map->collide(&x, &y, &vx, &vy, dt, 0.9);
    x += dt * vx;
    y += dt * vy;

    angle = 180. / M_PI * atan2(vy, vx);

    if(test)
        bounceCount += 1;
    if(bounceCount > MAX_BOUNCES)
        isDead = true;
    else{
        UpdateMessage msg(this);
        mgr << &msg;
    }
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

