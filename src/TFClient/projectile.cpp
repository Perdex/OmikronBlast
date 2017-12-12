#include "projectile.h"
#include "message.h"
#include "stuff.h"
#define RADIUS 10

projectile::projectile(qint16 id, double &x, double &y):stuff(id,x,y){}
projectile::~projectile(){}

void projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(getHorizontalPos(),getVerticalPos(),RADIUS/2,RADIUS/2);
}
QRectF projectile::boundingRect() const
{
    return QRectF(-RADIUS,-RADIUS,RADIUS*2,RADIUS*2);
}

stuff* projectile::create(qint16 id, QDataStream *str) {
    double x, y;

    str->startTransaction();

    *str >> x >> y;

    if(!str->commitTransaction()) return nullptr;

    return new projectile(id, x, y);
}
