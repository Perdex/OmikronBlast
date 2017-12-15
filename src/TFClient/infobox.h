#ifndef INFOBOX_H
#define INFOBOX_H


#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFont>
#include <QPointF>

#include "stuff.h"
#include "player.h"

#define ANGLE 0

class Infobox : public QGraphicsView
{
    Q_OBJECT
public:
    Infobox(QWidget* p = 0);
    void setMyPlayer(player*);
    void update();
    QString createScores();
    void addPlayer(player *);
    void countDown(int sec);

protected:
    QGraphicsScene* scene;
    QGraphicsPixmapItem *background;
    QGraphicsTextItem *ammo;
    QGraphicsTextItem *fuel;
    QGraphicsTextItem *score;
    QGraphicsTextItem *name;
    QGraphicsTextItem *scoreboard;
    QGraphicsTextItem *countdown;
    QVector <player*> players;
    player *my_player;
};
#endif // INFOBOX_H
