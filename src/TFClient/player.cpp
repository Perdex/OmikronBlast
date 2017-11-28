#include "player.h"
#include <QString>

player::player(QString name, QChar id, double& x, double& y): name(name), id(id), stuff(x,y)
{
    ammo = 0;
    fuel = 0;
    isMe = false;
    mouseClicked = 0;
    isDead = 0;
}
player::~player(){}

//type << " " << getIsDead() << " " << getHorizontalPos()
//     << " " << getVerticalPos() << " " << getAmmoLeft() << " " << fuel << " " << getId()
void player::decode(QString str)
{
    int i = 2;
    QString tester = "";
    int s = str.size();

    if(str[i] == '1' || str[i] == 't')
        isDead = 1;
    while(str[i]!=' ')
    {
        i++;
    }
    i++;
    while(str[i]!=' ')
    {
        tester.append(str[i]);
        i++;
    }
    setHorizontalPos(tester.toDouble());
    i++;
    tester = "";
    while(str[i]!=' ')
    {
        tester.append(str[i]);
        i++;
    }
    setVerticalPos(tester.toDouble());
    i++;
    ammo = str[i];
    i++;
    i++;
    fuel = str[i];
}

void player::draw()
{

}

QString player::encode()
{

}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::blue);
    painter->drawPixmap(0,0,50,100,QPixmap(":/images/Images/Marinestance.png"));
}
QRectF player::boundingRect() const
{
    return QRectF(0,0,50,100);
}

