#include "player.h"
#include <QString>
#include <QtDebug>

player::player(QString name, qint16 id, double x, double y)
    : stuff(Stuff::PLAYER,id,x,y), name(name)
{

    marine = QPixmap(":/images/Images/Marinestance_nogun.png");
    gun = QPixmap(":/images/Images/Marine_gun.png");
    flame = QPixmap(":/images/Images/flame2.png");
    stone = QPixmap(":/images/Images/stone.png");
}

player::~player(){}

int player::getAmmo() {return ammo;}
int player::getFuel() {return fuel;}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isDead) {
        painter->drawPixmap(-30, -15, 60, 60, stone);
        return;
    }
    double ang = angle;
    if(ang < -90 || ang > 90){
        //flip the image
        QTransform transf = painter->transform();
        transf.scale(-1, 1);
        painter->setTransform(transf);

        //transform angle to right side
        if(ang < 0)
            ang = -180 - ang;
        else
            ang = 180 - ang;
    }

    if(jetpackActive)
        painter->drawPixmap(-22,25,26,34, flame);


    painter->drawPixmap(-39,-50,78,100, marine);

    //draw the gun
    QTransform transf = painter->transform();
    transf.translate(-18, 10);
    transf.rotate(ang);
    painter->setTransform(transf);
    painter->drawPixmap(-10,-15,60,36, gun);

}
QRectF player::boundingRect() const
{
    return QRectF(-39,-50,78,110);
}

void player::setAngle(double angle){
    //this is currently always between -180, 180
    this->angle = angle;
}

void player::update(QDataStream *s)
{
    double hp, vp;

    s->startTransaction();

    int a,f;
    bool jp, d;
    *s >> hp >> vp >> d >> jp >> a >> f;

    if(!s->commitTransaction() || isDead) return;

    jetpackActive = jp;
    ammo = a;
    fuel = f;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
    this->setIsDead(d);
}

player* player::create(qint16 id, QDataStream *stream) {

    QString name = "";
    double hp, vp;

    stream->startTransaction();

    int a,f;
    bool jp, d;
    *stream >> hp >> vp >> d >> jp >> a >> f;

    if(!stream->commitTransaction() || d) return nullptr;

    return new player(name, id, hp, vp);
}
