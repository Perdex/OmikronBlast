#include "player.h"
#include "message.h"
#include "map.h"
#include "tcpmanager.h"
#include "projectile.h"
#include "mainwindow.h"
#include <QDataStream>
#include <QMap>
#include <QtDebug>

#define AMMOMAX 5
#define JETFUELMAX 100.0
#define GRAVITY 0.1
#define JETPACKPOWER -0.2
#define TERMINALSPEED 3.0
#define HORIZONTALMOVEMENT 0.20
#define JUMPSTRENGTH -2.0
#define COLLWIDTH 15

player::player(qint16 id, QString name, QDataStream *stream, Map *map, MainWindow *main)
    : stuff(Stuff::PLAYER, id, map, main, stream),
    name(name),
    ammoLeft(AMMOMAX),
    jetpackStatus(false),
    fuelLeft(JETFUELMAX),
    lastMagazineFull(0),
    lastJetpackUse(0),
    isFalling(true),
    score(0),
    aPressed(false),
    dPressed(false){

    resetPosition(map);
}

bool player::doStep()
{
    QMap<int, bool> map;
    bool clicked;
    double angle;
    bool flag;
    do{
        stream->startTransaction();

        *stream >> map >> clicked >> angle;

        // Set isFalling = false if player's feet touch the ground
        isFalling = !this->map->touches(x - COLLWIDTH + 1, y + 41)
                 && !this->map->touches(x + COLLWIDTH - 1, y + 41);
        flag = stream->commitTransaction();
        if(flag) {

            if(map[Qt::Key_W] && fuelLeft > 0)
            {
                jetpackStatus = true;
                lastJetpackUse = 0;
            }else
                jetpackStatus = false;

            // Use slightly lower values for jumping than falling for consistency
            // (the y-position will move slightly when "stationary")
            bool canJump = this->map->touches(x - COLLWIDTH + 1, y + 43)
                        || this->map->touches(x + COLLWIDTH - 1, y + 43);

            if(map[Qt::Key_Space] && canJump) jump();


            // Horizontal movement flags
            aPressed = map[Qt::Key_A];
            dPressed = map[Qt::Key_D];

            // Shooting functionality
            if(!isDead && clicked && ammoLeft > 0) {
                weaponAngle = angle;
                ammoLeft -= 1;
                shoot();
            }
        }
    }while(flag);
    return isDead;
}

void player::resetPosition(Map* m)
{
    map = m;
    int x, y;
    do{
        x = rand() % 40;
        y = rand() % 40;
    }while(map->isWall(x, y));
    this->x = x * 100 + 50;
    this->y = y * 100 + 50;
}

void player::move(int dt, TCPManager &mgr)
{
    //Ammoregen
    if(ammoLeft == 5)
        lastMagazineFull = 0;
    else if(lastMagazineFull >= 2000)
    {
        ammoLeft += 1;
        lastMagazineFull = 0;
    }
    else
        lastMagazineFull += dt;

    // Jetpack functionality
    if(!jetpackStatus) lastJetpackUse += dt;
    if(lastJetpackUse > 3000) fuelLeft = qMin(JETFUELMAX, fuelLeft + (dt/10));

    if(jetpackStatus && fuelLeft > 0){
        vy += JETPACKPOWER;
        fuelLeft = qMax(fuelLeft - (dt/20), 0.0);
    }

    // Horizontal movement
    if(aPressed) vx -= HORIZONTALMOVEMENT;
    if(dPressed) vx += HORIZONTALMOVEMENT;

    // Add gravity
    if(isFalling)
        vy += GRAVITY;

    // Damp velocities
    vx *= 0.8;
    vy *= 0.95;

    // Limit the velocity to values that make sense
    vx = qBound(-TERMINALSPEED, vx, TERMINALSPEED);
    vy = qBound(-TERMINALSPEED, vy, TERMINALSPEED);


    // Collide with map, use three collision points
    // Could be done better but whatever
    QPoint collPoints[] = {QPoint(-COLLWIDTH, 41), QPoint(COLLWIDTH, 41), QPoint(0, -40)};

    for(QPoint p: collPoints){
        // First, offset the points, the collide, then undo the offset
        double x_temp = x + p.x();
        double y_temp = y + p.y();
        map->collide(&x_temp, &y_temp, &vx, &vy, dt, 0.1);
        x = x_temp - p.x();
        y = y_temp - p.y();
    }

    // Move
    x += dt * vx;
    y += dt * vy;

    // Send the new state to clients
    UpdateMessage msg = UpdateMessage((stuff*)this);
    mgr << &msg;
}

player::~player(){}

int player::getAmmoLeft() const
{
    return ammoLeft;
}
double player::getFuelLeft() const
{
    return fuelLeft;
}
QString player::getName() const
{
    return name;
}

bool player::getJetpackStatus() const
{
    return jetpackStatus;
}

int player::getScore() const{
    return score;
}
void player::jump()
{
    vy = JUMPSTRENGTH;
}

void player::shoot()
{
    if(mainWindow->isRunning()){
        projectile *p = new projectile(this->mainWindow->getNextId(), this->x, this->y, this,
                                       weaponAngle, map, mainWindow);
        mainWindow->addProjectile(p);
    }
}
void player::die()
{
    isDead = true;
}
void player::undie()
{
    isDead = false;
}
void player::getPoint()
{
    score += 1;
}
