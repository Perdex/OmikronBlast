#include "projectile.h"
#include "message.h"
#include "stuff.h"
#define RADIUS 10

projectile::projectile(qint16 id, double &x, double &y)
    : stuff(id,x,y),
    angle(0){

    pixmap = QPixmap(":/images/Images/projectile.png");
}

projectile::~projectile(){}

void projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->setBrush(Qt::red);
    //painter->drawEllipse(-RADIUS,-RADIUS,RADIUS*2,RADIUS*2);

    QTransform transf = painter->transform();
    transf.rotate(angle);
    painter->setTransform(transf);

    painter->drawPixmap(-20, -7, 40, 14, pixmap);
}
QRectF projectile::boundingRect() const
{
    return QRectF(-RADIUS*2,-RADIUS*2,RADIUS*4,RADIUS*4);
}

void projectile::update(QDataStream *s) {
    double hp, vp;
    s->startTransaction();

    double last_x = getHorizontalPos();
    double last_y = getVerticalPos();

    *s >> hp >> vp;

    if(!s->commitTransaction()) return;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);

    double dx = hp - last_x;
    double dy = vp - last_y;
    angle = 180 + atan2(dy, dx) * 180. / M_PI;
}

stuff* projectile::create(qint16 id, QDataStream *str) {
    double x, y;

    str->startTransaction();

    *str >> x >> y;

    if(!str->commitTransaction()) return nullptr;

    return new projectile(id, x, y);
}
