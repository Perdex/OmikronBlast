#ifndef PLAYER_H
#define PLAYER_H
#include <ctime>
#include <iostream>
#include "stuff.h"
#include <QString>
#include <QtGlobal>
#include <QTextStream>
#include <QDataStream>

class Map;

class player : public stuff
{
public:
    player(QString name, qint16 id, double& x, double& y, bool dead, int ammoMax, double fuelMax,
           double angle);
    player(qint16 id, QDataStream *stream, Map* map);
    ~player();
    //QDataStream& operator<<(QDataStream& stream);

    bool getIsDead()const;
    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    int getAmmoMax() const;
    double getFuelMax() const;
    QString getName() const;
    int getLastMagazineFull() const;
    int getLastJetpackUse() const;
    void jump();
    void doStep(int dt);
    void move(int dt, TCPManager &mgr);
    void shoot(double angle);
    void die();

private:
    QString name;
    bool isDead;
    int width;
    int height;
    int ammoLeft;
    int ammoMax;
    bool jetpackStatus;
    double fuelLeft;
    double fuelMax;
    time_t lastMagazineFull;
    time_t lastJetpackUse;
    double weaponAngle;
    bool isFalling;
};

#endif // PLAYER_H
