#ifndef PLAYER_H
#define PLAYER_H
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
    player(qint16 id, QDataStream *stream, Map* map, MainWindow *main);
    ~player();
    //QDataStream& operator<<(QDataStream& stream);

    bool getIsDead()const;
    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    double getFuelLeft() const;
    QString getName() const;
    bool getJetpackStatus() const;
    int getScore() const;
    void jump();
    bool doStep(int dt);
    void move(int dt, TCPManager &mgr);
    void shoot();
    void die();
    void getPoint();
    void resetPosition();

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
