#include "player.h"
#include <QString>
#include <QtDebug>

player::player(QString name, qint16 id, double x, double y): name(name), stuff(id,x,y)
{
    ammo = 0;
    fuel = 0;
    isMe = false;
    mouseClicked = 0;
    isDead = 0;
    angle = 0.0;
}
player::~player(){}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //unnecessary?
    //painter->setBrush(Qt::blue);

    static QPixmap pic1a = QPixmap(":/images/Images/Marinestance.png");
    static QPixmap pic2a = QPixmap(":/images/Images/Marinestance_2.png");
    //make a flipping transformation. Or make the weapon as a separate image and rotate it to correct angle
    //static QPixmap pic1b = pic1a.transformed(QTransform());
    //static QPixmap pic2b = pic2a.transformed(QTransform());

    //if angle points up, use pic2: if angle is to the left, use mirror images
    painter->drawPixmap(-25,-50,50,100,pic1a);
}
QRectF player::boundingRect() const
{
    return QRectF(-25,-50,50,100);
}

void player::update(QDataStream *s)
{
    double hp, vp;
    s->startTransaction();

    *s >> hp >> vp;

    if(!s->commitTransaction()) return;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
}
