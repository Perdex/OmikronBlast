#include "player.h"
#include <QString>
#include <QtDebug>

player::player(QString name, qint16 id, double x, double y): name(name), stuff(Stuff::PLAYER,id,x,y)
{
    isDead = 0;

    marine = QPixmap(":/images/Images/Marinestance_nogun.png");
    gun = QPixmap(":/images/Images/Marine_gun.png");
    flame = QPixmap(":/images/Images/flame.png");
    stone = QPixmap(":/images/Images/stone.png");
}

player::~player(){}

int player::getAmmo() {return ammo;}
int player::getFuel() {return fuel;}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isDead) {
        painter->drawPixmap(-39, -50, 78, 100, stone);
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
        painter->drawPixmap(-39,0,78,50, flame);


    painter->drawPixmap(-39,-50,78,100, marine);

    //draw the gun
    QTransform transf = painter->transform();
    transf.translate(-18, 3);
    transf.rotate(ang);
    painter->setTransform(transf);
    painter->drawPixmap(-10,-15,60,36, gun);

}
QRectF player::boundingRect() const
{
    return QRectF(-39,-50,78,100);
}

void player::setAngle(double angle){
    qDebug() << angle;
    //this is currently always between -180, 180
    this->angle = angle;
}

void player::update(QDataStream *s)
{
    double hp, vp;

    s->startTransaction();

    int a,f;
    bool jp, d;
    *s >> hp >> vp >> jp >> a >> f >> d;

    if(!s->commitTransaction() || isDead) return;

    jetpackActive = jp;
    ammo = a;
    fuel = f;
    isDead = d;
    qDebug() << isDead;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
}

player* player::create(qint16 id, QDataStream *stream) {

    QString name = "";
    double hp, vp;

    stream->startTransaction();

    int a,f;
    bool jp, d;
    *stream >> hp >> vp >> jp >> a >> f >> d;

    if(!stream->commitTransaction()) return nullptr;

    return new player(name, id, hp, vp);
}
