#include "player.h"
#include "stuff.h"
#include "message.h"
#include "tcpmanager.h"
#include <QDataStream>
#include <QMap>
#include <QtDebug>
#define AMMOMAX 5
#define JETFUELMAX 100.0
#define PLAYERWIDTH 20
#define PLAYERHEIGHT 40
#define GRAVITY 0.8
#define JETPACKPOWER -1.6
#define TERMINALSPEED 1000.0
#define HORIZONTALMOVEMENT 500.0
#define JUMPSTRENGTH -800.0
#define FUELCONSUMPTION 5.0

player::player(QString name, qint16 id, double& x, double& y, bool dead = 0, int ammoMax = AMMOMAX, double fuelMax = JETFUELMAX,
               double angle = 0) :
    stuff(id, x,y,0,0), name(name),  isDead(dead), ammoMax(ammoMax), fuelMax(fuelMax), weaponAngle(angle)
{
    width = PLAYERWIDTH;
    height = PLAYERHEIGHT;
    ammoLeft = AMMOMAX;
    jetpackStatus = false;
    fuelLeft = JETFUELMAX;
    lastMagazineFull = 0;
    lastJetpackUse = 0;
    isFalling = 0;
    acceleration = 0.0;
}

player::player(qint16 id, QDataStream *stream)
    : stuff(Stuff::PLAYER, id, stream),
    name("NONAME_SUCKER"),
    isDead(false){
    setVerticalPos(2500);
    setHorizontalPos(2500);
}

void player::doStep(int dt)
{
    QMap<int, bool> map;
    bool clicked;
    double angle;

     //qDebug() << stream->status();

    stream->startTransaction();

    *stream >> map >> clicked >> angle;

    //qDebug() << map << clicked << angle;

    if(stream->commitTransaction()) {
        qDebug() << map;

        if(map[Qt::Key_S]) isFalling = true; //tähän unit collision kunhan saadaan
        else isFalling = false;

        if(map[Qt::Key_W] && fuelLeft > 0)
        {
            jetpackStatus = true;
        }
        else jetpackStatus = false;

        if(map[Qt::Key_Space]) jump(); // tähän vielä !isFalling kunha saadaan unit collision

        if(isFalling && !jetpackStatus) acceleration = GRAVITY;
        else if(!isFalling && jetpackStatus) acceleration = JETPACKPOWER;
        else if(isFalling && jetpackStatus) acceleration = JETPACKPOWER + GRAVITY;
        else acceleration = 0;


        if(map[Qt::Key_A] && !map[Qt::Key_D]) setHorizontalSpeed(-HORIZONTALMOVEMENT);
        else if(map[Qt::Key_D] && !map[Qt::Key_A]) setHorizontalSpeed(HORIZONTALMOVEMENT);
        else setHorizontalSpeed(0);

    }
}

void player::move(int dt, TCPManager &mgr)
{
    changeVerticalSpeed(dt * acceleration);

    setVerticalSpeed(qMin(getVerticalSpeed(),TERMINALSPEED));

    changeVerticalPos(getVerticalSpeed() * dt / 1000);
    changeHorizontalPos(getHorizontalSpeed() * dt / 1000);

    mgr << (Message*)(new UpdateMessage((stuff*)this));
}

player::~player(){}

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
double player::getFuelMax() const
{
    return fuelMax;
}
QString player::getName() const
{
    return name;
}

int player::getLastMagazineFull() const
{
    return lastMagazineFull;
}
int player::getLastJetpackUse() const
{
    return lastJetpackUse;
}
void player::jump()
{
        setVerticalSpeed(JUMPSTRENGTH);
}

void player::shoot(double angle)
{

}
void player::die()
{
    isDead = true;
}
