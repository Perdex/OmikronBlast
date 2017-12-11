#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <iostream>
#include "stuff.h"
#include <QString>
#include <QtGlobal>
#include <QDataStream>
#include "player.h"


class projectile : public stuff
{
public:
    projectile(qint16 id, double& x, double& y, player& owner,  double angle);
    ~projectile();

    void move();
    void hitPlayer(player& victim);

private:
    int bounceCount;
    player& owner;
};

#endif // PROJECTILE_H
