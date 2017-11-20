#include "stuff.h"

stuff::stuff(double& x, double& y, double dx = 0 , double dy = 0) : horizontalPos(x), verticalPos(y),
             horizontalSpeed(dx), verticalSpeed(dy){}
stuff::~stuff(){}
double stuff::getVerticalPos() const
{
    return verticalPos;
}
double stuff::getHorizontalPos() const
{
    return horizontalPos;
}
double stuff::getVerticalSpeed() const
{
    return verticalSpeed;
}
double stuff::getHorizontalSpeed() const
{
    return horizontalSpeed;
}
void stuff::changeVerticalSpeed(double change)
{
    verticalSpeed += change;
}
void stuff::changeHorizontalSpeed(double change)
{
    horizontalSpeed += change;
}
void stuff::setVerticalSpeed(double speed)
{
    verticalSpeed = speed;
}
void stuff::setHorizontalSpeed(double speed)
{
    horizontalSpeed = speed;
}
void stuff::changeHorizontalPos(double change)
{
    horizontalPos += change;
}
void stuff::changeVerticalPos(double change)
{
    verticalPos += change;
}
