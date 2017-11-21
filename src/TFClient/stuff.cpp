#include "stuff.h"
#include <QString>

stuff::stuff(double& x, double& y): horizontalPos(x), verticalPos(y)
{

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
