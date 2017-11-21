#include "player.h"
#include <QString>

player::player(std::string name, char id, double& x, double& y): name(name), id(id), stuff(x,y)
{
    ammo = '0';
    fuel = '0';
    isMe = FALSE;
    mouseClicked = '0';
}
player::~player(){}

void player::decode(QString i)
{

}

void player::draw()
{

}

QString player::encode()
{

}

