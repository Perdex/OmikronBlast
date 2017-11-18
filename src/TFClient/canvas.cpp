#include "canvas.h"


#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p)
{
    scene = new QGraphicsScene(0,0,800,800, this);
    this->setScene(scene);
    //scene->addLine(5000, 5000, 5100, 5400);
   // scene->invalidate();
    Item *a = new Item();
    scene->addItem(a);
    QImage image("C:\\Users\\essi\\towerfall-ascension\\src\\TFClient\\Images\\background.png");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    this->centerOn(centerX, centerY);
}

void Canvas::mouseMoveEvent(QMouseEvent * me) {
    this->centerOn(centerX + (me->x() - this->width()/2)/2, centerY + (me->y() - this->height()/2)/2);
    qDebug() << centerX + me->x() - this->width()/2;
}
