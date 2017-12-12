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
    projectile(qint16 id, double x, double y, player *owner, double angle,
                           Map *map, MainWindow *main);
    ~projectile();

    bool doStep(int dt);
    void move(int dt, TCPManager &mgr);
    void hitPlayer(player& victim);
private slots:
    void activate() { isActive = true; }
private:
    int bounceCount;
    player *owner;
    bool isActive = false;
    double getAngle();
};

#endif // PROJECTILE_H
