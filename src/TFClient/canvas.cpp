#include "canvas.h"

#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p)
{
    scene = new QGraphicsScene(0,0,8000,8000, this);
    this->setScene(scene);
    scene->addLine(5000, 5000, 5100, 5400);
    scene->invalidate();

    this->centerOn(centerX, centerY);
}

void Canvas::mouseMoveEvent(QMouseEvent * me) {
    this->centerOn(centerX + (me->x() - this->width()/2)/2, centerY + (me->y() - this->height()/2)/2);
    qDebug() << centerX + me->x() - this->width()/2;
}
