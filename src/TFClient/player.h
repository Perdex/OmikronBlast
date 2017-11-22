#ifndef PLAYER_H
#define PLAYER_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>

class player :public stuff
{
public:
    player(QString, QChar, double&, double&);
    ~player();
    void decode(QString);
    void draw();
    QString encode();

private:
    QString name;
    QChar id;
    bool isDead;
    QChar ammo;
    QChar fuel;
    bool isMe;
    QChar mouseClicked;
};

#endif // PLAYER_H
