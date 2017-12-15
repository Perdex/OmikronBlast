#include "player.h"
#include <QString>
#include <QtDebug>

player::player(QString name, qint16 id, double x, double y, int score)
    : stuff(Stuff::PLAYER,id,x,y),
      name(name), ammo(5), fuel(100),
      score(score), jetpackActive(false)
{

    marine = QPixmap(":/images/Images/Marinestance_nogun.png");
    gun = QPixmap(":/images/Images/Marine_gun.png");
    flame = QPixmap(":/images/Images/flame2.png");
    stone = QPixmap(":/images/Images/stone.png");
}

player::~player(){}

int player::getAmmo() {return ammo;}
int player::getFuel() {return fuel;}
int player::getScore() {return score;}
QString player::getName() {return name;}
bool player::operator<(player *p)
{
    bool s = getScore() < p->getScore();
    return s;
}

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

    int a,f,c;
    bool jp, d;
    QString name_;
    *s >> hp >> vp >> d >> name_ >> jp >> a >> f >> c;

    if(!s->commitTransaction() || isDead) return;

    jetpackActive = jp;
    ammo = a;
    fuel = f;
    score = c;
    name = name_;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
    this->setIsDead(d);
}

player* player::create(qint16 id, QDataStream *stream) {

    QString name;
    double hp, vp;

    stream->startTransaction();

    int ammo,fuel,score;
    bool jp, dead;
    *stream >> hp >> vp >> dead >> name >> jp >> ammo >> fuel >> score;

    if(!stream->commitTransaction() || dead) return nullptr;

    return new player(name, id, hp, vp, score);
}
