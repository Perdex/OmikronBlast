#ifndef PLAYER_H
#define PLAYER_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>

class player :public stuff
{
public:
    player(std::string, char, double&, double&);
    ~player();
    void decode(QString);
    void draw();
    QString encode();

private:
    std::string name;
    char id;
    char ammo;
    char fuel;
    bool isMe;
    char mouseClicked;
};

#endif // PLAYER_H
