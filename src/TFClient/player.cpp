#include "player.h"
#include <QString>
#include <QtDebug>
#include <QSound>

player::player(QString name, qint16 id, double x, double y, int score)
    : stuff(Stuff::PLAYER,id,x,y),
      name(name), ammo(5), fuel(100), angle(0),
      score(score), jetpackActive(false), onGround(false), jetpack(":/sounds/Sounds/rocket.wav")
{
    jetpack.setLoops(QSound::Infinite);
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

//looks funny but propably easiest way to get descending order
bool player::operator<(player *p)
{
    bool s = getScore() >= p->getScore();
    return s;
}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(isDead) {
        painter->drawPixmap(-35, -25, 70, 70, stone);
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

    if(jetpackActive) {
        painter->drawPixmap(-22,25,26,34, flame);
    }

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
    //hp = horizontal position
    //vp = vertical postition

    s->startTransaction();

    int a,f,c;
    bool jp, d, canJump;
    //a = ammo
    //f = fuel
    //c = score
    //jp tells wether jetpack is active or not
    //d tells if player is dead

    QString name_;
    *s >> hp >> vp >> d >> name_ >> jp >> a >> f >> c >> canJump;

    if(!s->commitTransaction()) return;

    if(!jetpackActive && jp) {
        jetpack.play();
    }

    if(!jp) {
        jetpack.stop();
    }

    jetpackActive = jp;
    ammo = a;
    fuel = f;
    score = c;
    name = name_;
    isDead = d;
    onGround = canJump;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
}

player* player::create(qint16 id, QDataStream *stream) {

    QString name;
    double hp, vp;
    //hp = horizontal position
    //vp = vertical postition

    stream->startTransaction();

    int ammo,fuel,score;
    bool jp, dead, canJump;
    //jp tells wether jetpack is active or not
    *stream >> hp >> vp >> dead >> name >> jp >> ammo >> fuel >> score >> canJump;

    if(!stream->commitTransaction() || dead) return nullptr;

    return new player(name, id, hp, vp, score);
}
