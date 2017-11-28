#ifndef PLAYER_H
#define PLAYER_H
#include <ctime>
#include <iostream>
#include "stuff.h"
#include <QString>
#include <QtGlobal>
#include <QTextStream>
#include <QDataStream>


class player : public stuff
{
public:
    player(QString, QChar, double&, double&, bool, int, double,  double);
    player(QDataStream *stream): stuff(stream){}//TODO
    ~player();
    bool getIsDead()const;
    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    int getAmmoMax() const;
    bool getJetpackStatus() const;
    double getFuelLeft() const;
    double getFuelMax() const;
    QString getName() const;
    QChar getId() const;
    time_t getLastMagazineFull() const;
    time_t getLastJetpackUse() const;
    QDataStream& encode(QDataStream&);
    void decode(QString);
  public slots:
    void startFall();
    void stopFall();
    void jump();
    void move(int dt, TCPManager *mgr);
    void shoot();
    void die();

private:
    QString name;
    QChar id;
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
    bool goingLeft;
    bool goingRight;
};

#endif // PLAYER_H
