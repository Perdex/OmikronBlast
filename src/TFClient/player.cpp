#include "player.h"
#include <Windows.h>
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
    QString code ="";
    if(GetKeyState('A') & 0x8000)
    {
        code.append("1");
    }
    else
    {
        code.append("0");
    }

    if(GetKeyState('W') & 0x8000)
    {
        code.append("1");
    }
    else
    {
        code.append("0");
    }
    if(GetKeyState('D') & 0x8000)
    {
        code.append("1");
    }
    else
    {
        code.append("0");
    }
    if(GetKeyState(VK_SPACE) & 0x8000)
    {
        code.append("1");
    }
    else
    {
        code.append("0");
    }
    if((GetKeyState(VK_LBUTTON) & 0x8000) && mouseClicked == '0')
    {
       mouseClicked ='1';
       code.append("1");
    }
    else if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
       mouseClicked = '-1';
       code.append("0");
    }
    else
    {
       mouseClicked = '0';
       code.append("0");
    }
    return code;
}

