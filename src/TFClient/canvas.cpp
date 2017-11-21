#include "canvas.h"

#include <QDir>
#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p)
{
    scene = new QGraphicsScene(0,0,4000,4000, this);
    this->setScene(scene);
    QPixmap pm(":/images/Images/background.png");
    pm = pm.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    item->setPixmap(pm);
    scene->addItem(item);
    this->centerOn(centerX, centerY);

    scene->invalidate();
}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    this->centerOn(centerX + (me->x() - this->width()/2)/2, centerY + (me->y() - this->height()/2)/2);
}
