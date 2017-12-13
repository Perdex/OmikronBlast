#include "stuff.h"
#include "message.h"
#include "player.h"
#include "projectile.h"
#include <QString>
#include <QtDebug>

stuff::stuff(Stuff t, qint16 id, double& x, double& y)
    : type(t), id(id), horizontalPos(x), verticalPos(y)
{
    setPos(horizontalPos, verticalPos);
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

stuff* stuff::create(UpdateMessage *msg, QDataStream *stream) {
    switch(msg->datatype()) {
    case Stuff::PLAYER:
        return player::create(msg->id(), stream);
    case Stuff::PROJECTILE: {
        return projectile::create(msg->id(), stream);
    }
    default:
        return nullptr;
    }
}
