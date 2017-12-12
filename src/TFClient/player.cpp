#include "player.h"
#include <QString>
#include <QtDebug>

player::player(QString name, qint16 id, double x, double y): name(name), stuff(id,x,y)
{
    isMe = false;
    isDead = 0;

    pixmaps[0] = QPixmap(":/images/Images/Marinestance.png");
    pixmaps[1] = QPixmap(":/images/Images/Marinestance_2.png");
    pixmaps[2] = QPixmap(":/images/Images/Marinestance_3.png");
    flame = QPixmap(":/images/Images/flame.png");
}

player::~player(){}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

    if(jetpackActive){
        painter->drawPixmap(-39,-50,78,100,flame);
    }
    int ind = 0;
    if(ang < -30)
        ind = 1;
    if(ang > 30)
        ind = 2;

    painter->drawPixmap(-39,-50,78,100,pixmaps[ind]);
}
QRectF player::boundingRect() const
{
    return QRectF(-39,-50,78,100);
}

void player::setAngle(double angle){
    //qDebug() << angle;
    //this is currently always between -180, 180
    this->angle = angle;
}

void player::update(QDataStream *s)
{
    double hp, vp;

    s->startTransaction();

    int a,f;
    bool jp;
    *s >> hp >> vp >> jp >> a >> f;

    if(!s->commitTransaction()) return;

    jetpackActive = jp;
    ammo = a;
    fuel = f;

    this->setVerticalPos(vp);
    this->setHorizontalPos(hp);
}

player* player::create(qint16 id, QDataStream *stream) {

    QString name = "";
    double hp, vp;

    stream->startTransaction();

    int a,f;
    bool jp;
    *stream >> hp >> vp >> jp >> a >> f;

    if(!stream->commitTransaction()) return nullptr;

    return new player(name, id, hp, vp);
}
