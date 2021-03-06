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

/*This class is used to create the box in the ui that shows player all the information they're allowed to have
 * including their name, ammo, fuel, score and overall scoreboard.
 */

class Infobox : public QGraphicsView
{
    Q_OBJECT
public:
    Infobox(QWidget* p = 0);
    void setMyPlayer(player*);
    void update(QMap<qint16, stuff*> items);
    QString createScores(QMap<qint16, stuff*> items);
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
    player *my_player;
};
#endif // INFOBOX_H
