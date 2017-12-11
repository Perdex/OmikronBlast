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
#define PLAYERWIDTH 20
#define PLAYERHEIGHT 40
#define GRAVITY 0.1
#define JETPACKPOWER -0.15
#define TERMINALSPEED 3.0
#define HORIZONTALMOVEMENT 0.2
#define JUMPSTRENGTH -2.0
#define FUELCONSUMPTION 5.0
#define COLLWIDTH 15

player::player(qint16 id, QDataStream *stream, Map *map, MainWindow *main)
    : stuff(Stuff::PLAYER, id, map, main, stream),
    name("NONAME_SUCKER"),
    isDead(false),
    ammoLeft(3),
    jetpackStatus(false),
    fuelLeft(JETFUELMAX),
    lastMagazineFull(0),
    lastJetpackUse(0),
    isFalling(true),
    score(0),
    aPressed(false),
    dPressed(false){

    int x, y;
    do{
        x = rand() % 40;
        y = rand() % 40;
    }while(map->isWall(x, y));
    this->x = x * 100 + 50;
    this->y = y * 100 + 50;
}

void player::doStep(int dt)
{
    QMap<int, bool> map;
    bool clicked;
    double angle;


    stream->startTransaction();

    *stream >> map >> clicked >> angle;

    isFalling = !this->map->touches(x - COLLWIDTH + 2, y + 41)
             && !this->map->touches(x + COLLWIDTH - 2, y + 41);
    if(stream->commitTransaction()) {
        //qDebug() << map;

        if(map[Qt::Key_W] && fuelLeft > 0)
        {
            jetpackStatus = true;
            lastJetpackUse = 0;
        }else
            jetpackStatus = false;


        if(map[Qt::Key_Space] && !isFalling) jump(); // tähän vielä !isFalling kunhan saadaan unit collision


        aPressed = map[Qt::Key_A];
        dPressed = map[Qt::Key_D];

        //if(clicked)
        //    shoot();
    }
}

void player::move(int dt, TCPManager &mgr)
{

    if(!jetpackStatus) lastJetpackUse += dt;
    if(lastJetpackUse > 3000) fuelLeft = qMin(JETFUELMAX, fuelLeft + (dt/10));

    if(jetpackStatus && fuelLeft > 0){
        vy += JETPACKPOWER;
        fuelLeft = qMax(fuelLeft - (dt/10), 0.0);
    }

    if(aPressed) vx -= HORIZONTALMOVEMENT;
    if(dPressed) vx += HORIZONTALMOVEMENT;

    if(isFalling)
        vy += GRAVITY;

    vx *= 0.8;
    vy *= 0.95;

    // Limit the velocity to values that make sense
    vx = qBound(-TERMINALSPEED, vx, TERMINALSPEED);
    vy = qBound(-TERMINALSPEED, vy, TERMINALSPEED);

    // Collide with map
    double x_new = x - COLLWIDTH;
    double y_new = y + 40;
    map->collide(&x_new, &y_new, &vx, &vy, dt, 0.1);
    x_new += COLLWIDTH * 2;
    map->collide(&x_new, &y_new, &vx, &vy, dt, 0.1);
    x = x_new - COLLWIDTH;
    y = y_new - 40;

    x += dt * vx;
    y += dt * vy;

    Message *msg = new UpdateMessage((stuff*)this);
    mgr << msg;
    //TODO needs to be deleted, but deleting here crashes ht server...
}

player::~player(){}

bool player::getIsDead() const
{
    return isDead;
}
int player::getWidth() const
{
    return PLAYERWIDTH;
}
int player::getHeight() const
{
    return PLAYERHEIGHT;
}
int player::getAmmoLeft() const
{
    return ammoLeft;
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
    if(ammoLeft > 0){
        ammoLeft--;
        projectile *p = new projectile(this->mainWindow->getNextId(), this->x, this->y, this,
                                       weaponAngle, map, mainWindow);
        mainWindow->addProjectile(p);
    }
}
void player::die()
{
    isDead = true;
}
void player::getPoint()
{
    score += 1;
}
