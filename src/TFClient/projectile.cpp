#include "projectile.h"
#include "message.h"
#include "stuff.h"
#define RADIUS 10

projectile::projectile(qint16 id, double &x, double &y):stuff(id,x,y){
    ball = QPixmap(RADIUS *2, RADIUS*2);
    ball.fill(QColor("red"));
}
projectile::~projectile(){}

void projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    path.addEllipse(getHorizontalPos(),getVerticalPos(),RADIUS/2,RADIUS/2);
    painter->setClipPath(path);
    painter->drawPixmap(getHorizontalPos(),getVerticalPos(),RADIUS/2,RADIUS/2, ball);
}
QRectF projectile::boundingRect() const
{
    return QRectF(-RADIUS,-RADIUS,RADIUS*2,RADIUS*2);
}

void projectile::update(QDataStream *s) {
    double hp, vp;
    s->startTransaction();

    *s >> hp >> vp;

    if(!s->commitTransaction()) return;


    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
}

stuff* projectile::create(qint16 id, QDataStream *str) {
    double x, y;

    str->startTransaction();

    *str >> x >> y;

    if(!str->commitTransaction()) return nullptr;

    return new projectile(id, x, y);
}
