#ifndef INFOBOX_H
#define INFOBOX_H


#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "stuff.h"
#include "player.h"

#define ANGLE 0

class Infobox : public QGraphicsView
{
    Q_OBJECT
public:
    Infobox(QWidget* p = 0);
protected:
    QGraphicsScene* scene;
    QGraphicsPixmapItem *background;
};
#endif // INFOBOX_H
