#include "stuff.h"
#include <QString>

stuff::stuff(double& x, double& y): horizontalPos(x), verticalPos(y)
{
    setPos(horizontalPos, verticalPos);
}

void stuff::decodeType(QString i)
{
//    char checker = i[0];
//    if(checker == '0')
//        player.decode(i);
//    else if(checker =='1')
//      projectile.decode(i);
}

stuff::~stuff(){}



double stuff::getHorizontalPos()
{
    return horizontalPos;
}

double stuff::getVerticalPos()
{
    return verticalPos;
}

void stuff::setHorizontalPos(double nw)
{
    horizontalPos = nw;
    setPos(nw, verticalPos);
}

void stuff::setVerticalPos(double nw)
{
    verticalPos = nw;
    setPos(horizontalPos, nw);
}

QDataStream& operator>>(QDataStream &stream, stuff *s) {
    double hp, vp;

    stream.startTransaction();
    stream >> hp >> vp;
    if(!stream.commitTransaction()) return stream;

    s->setHorizontalPos(hp);
    s->setVerticalPos(vp);

    //TODO player(+projectile)-spesifiä juttua

    return stream;
}
