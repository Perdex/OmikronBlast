#include "player.h"
#include "stuff.h"
#define AMMOMAX 5
#define JETFUELMAX 100.0
#define PLAYERWIDTH 20
#define PLAYERHEIGHT 40
#define GRAVITY 10.0
#define JETPACKPOWER 20.0
#define TERMINALSPEED 100.0
#define HORIZONTALMOVEMENT 20.0
#define JUMPSTRENGTH 30.0
#define FUELCONSUMPTION 5.0

player::player(QString name, QChar id, double& x, double& y, bool dead = 0, int ammoMax = AMMOMAX, double fuelMax = JETFUELMAX,
               double angle = 0) :
    stuff(x,y,0,0), name(name), id(id), isDead(dead), ammoMax(ammoMax), fuelMax(fuelMax), weaponAngle(angle)
{
    width = PLAYERWIDTH;
    height = PLAYERHEIGHT;
    ammoLeft = AMMOMAX;
    jetpackStatus = 0;
    fuelLeft = JETFUELMAX;
    lastMagazineFull = 0;
    lastJetpackUse = 0;
    isFalling = 0;
    goingLeft = 0;
    goingRight = 0;
}
player::~player(){}

QDataStream& player::operator<<(QDataStream& stream)
{
    QChar type = '0';
    QChar fuel = ((int)(getFuelLeft() + 0.5)) + '0';
    stream << getId() << type << getIsDead() << getHorizontalPos() << getVerticalPos() << getAmmoLeft() << fuel;
    return stream;
}

bool player::getIsDead() const
{
    return isDead;
}
int player::getWidth() const
{
    return width;
}
int player::getHeight() const
{
    return height;
}
int player::getAmmoLeft() const
{
    return ammoLeft;
}
int player::getAmmoMax() const
{
    return ammoMax;
}
bool player::getJetpackStatus() const
{
    return jetpackStatus;
}
double player::getFuelLeft() const
{
    return fuelLeft;
}
double player::getFuelMax() const
{
    return fuelMax;
}
QString player::getName() const
{
    return name;
}
QChar player::getId() const
{
    return id;
}
int player::getLastMagazineFull() const
{
    return lastMagazineFull;
}
int player::getLastJetpackUse() const
{
    return lastJetpackUse;
}

void player::decode(QString str)
{
    if(str[0] == str[1])
    {
        goingLeft = 0;
        goingRight = 0;
    }
    else if(str[0]==1)
    {
        goingLeft = 1;
        goingRight = 0;
    }
    else
    {
        goingLeft = 0;
        goingRight = 1;
    }

    if(str[2] == 1)
        jump();

    if(str[3] == 1 && fuelLeft != 0)
    {
        fuelLeft = qMax(0.0, fuelLeft - FUELCONSUMPTION);
        jetpackStatus = 1;
    }
    else
    {
        jetpackStatus = 0;
    }

    if(str[4] == 1)
    {
        shoot();
    }
}

void player::startFall()
{
    isFalling = 1;
}
void player::stopFall()
{
    isFalling = 0;
}
void player::jump()
{
    if(!isFalling)
        setVerticalSpeed(JUMPSTRENGTH);
}
void player::move(int dt, TCPManager* mgr)
{
    if(jetpackStatus)
        setVerticalSpeed(JETPACKPOWER);
    else if(isFalling)
        changeVerticalSpeed(-GRAVITY);

    if(goingRight && !goingLeft)
        setHorizontalSpeed(HORIZONTALMOVEMENT);
    else if(!goingRight && goingLeft)
        setHorizontalSpeed(-HORIZONTALMOVEMENT);
    else
        setHorizontalSpeed(0);

    changeVerticalPos(getVerticalSpeed());
    changeHorizontalPos(getHorizontalSpeed());
}

void player::shoot()
{

}
void player::die()
{
    isDead = true;
}
