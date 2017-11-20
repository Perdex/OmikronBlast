#ifndef PLAYER_H
#define PLAYER_H
#include <ctime>
#include <iostream>
#include "stuff.h"


class player : public stuff
{
public:
    player(std::string, char, double&, double&, bool, int, double,  double);
    ~player();
    bool getIsDead()const;
    int getWidth() const;
    int getHeight() const;
    int getAmmoLeft() const;
    int getAmmoMax() const;
    bool getJetpackStatus() const;
    double getFuelLeft() const;
    double getFuelMax() const;
    std::string getName() const;
    char getId() const;
    time_t getLastMagazineFull() const;
    time_t getLastJetpackUse() const;
    std::ostream& encode(std::ostream&);
  public slots:
    void goLeft();
    void stopLeft();
    void goRight();
    void stopRight();
    void startFall();
    void stopFall();
    void jump();
    void move();
    void activateJetpack();
    void deactivateJetpack();
    void shoot();
    void die();

private:
    std::string name;
    char id;
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
