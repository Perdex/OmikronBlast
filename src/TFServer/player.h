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
    player(qint16 id, QString name, Map* map, MainWindow *main);
    ~player();
    //QDataStream& operator<<(QDataStream& stream);

    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    double getFuelLeft() const;
    QString getName() const;
    bool getJetpackStatus() const;
    int getScore() const;
    void jump();
    bool doStep();
    bool receiveData(QDataStream &stream);
    void move(int dt, UDPManager &mgr);
    void shoot();
    void die();
    void undie();
    void getPoint();
    void resetPosition(Map*);

private:
    QString name;
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
