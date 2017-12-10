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
    player(qint16 id, QDataStream *stream, Map* map);
    ~player();
    //QDataStream& operator<<(QDataStream& stream);

    bool getIsDead()const;
    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    QString getName() const;
    bool getJetpackStatus() const;
    void jump();
    void doStep(int dt);
    void move(int dt, TCPManager &mgr);
    void shoot(double angle);
    void die();

private:
    QString name;
    bool isDead;
    int ammoLeft;
    bool jetpackStatus;
    double fuelLeft;
    int lastMagazineFull;
    int lastJetpackUse;
    double weaponAngle;
    bool isFalling;
    int score;
    bool aPressed;
    bool dPressed;
};

#endif // PLAYER_H
