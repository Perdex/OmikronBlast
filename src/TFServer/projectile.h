#ifndef PROJECTILE_H
#define PROJECTILE_H
<<<<<<< HEAD


class projectile:public stuff   //onko stuff??
{
public:
    projectile();
    ~projectile();
    int bounceNumber();

private:
    int bounces;
=======
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
    double getAngle();
private slots:
    void activate() { isActive = true; }
private:
    int bounceCount;
    player *owner;
    bool isActive = false;
    double angle;
>>>>>>> 2e2e86e5e33e6d28ead92caff7782e68fa8b08a3
};

#endif // PROJECTILE_H
