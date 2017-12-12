#include "projectile.h"
#include "message.h"
#include "stuff.h"
#define RADIUS 10

projectile::projectile(qint16 id, double &x, double &y, double angle)
    : stuff(id,x,y),
    angle(angle){

    pixmap = QPixmap(":/images/Images/projectile.png");
}

projectile::~projectile(){}

void projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->setBrush(Qt::red);
    //painter->drawEllipse(-RADIUS,-RADIUS,RADIUS*2,RADIUS*2);

    QTransform transf = painter->transform();
    transf.rotate(180 + angle);
    painter->setTransform(transf);

    painter->drawPixmap(-20, -7, 40, 14, pixmap);
}
QRectF projectile::boundingRect() const
{
    return QRectF(-RADIUS*2,-RADIUS*2,RADIUS*4,RADIUS*4);
}

void projectile::update(QDataStream *s) {
    double hp, vp, ang;
    s->startTransaction();

    *s >> hp >> vp >> ang;

    if(!s->commitTransaction()) return;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);

    angle = ang;
}

stuff* projectile::create(qint16 id, QDataStream *str) {
    double x, y, angle;

    str->startTransaction();

    *str >> x >> y >> angle;

    if(!str->commitTransaction()) return nullptr;

    return new projectile(id, x, y, angle);
}
