#ifndef CANVAS_H
#define CANVAS_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "item.h"

class Canvas : public QGraphicsView
{
public:
    Canvas(QWidget* p = 0);
protected:
    void mouseMoveEvent(QMouseEvent*);
    QGraphicsScene* scene;
    int centerX = 1500;
    int centerY = 1500;
};

#endif // CANVAS_H
