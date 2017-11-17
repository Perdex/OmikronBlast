#ifndef CANVAS_H
#define CANVAS_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

class Canvas : public QGraphicsView
{
public:
    Canvas(QWidget* p = 0);
protected:
    void mouseMoveEvent(QMouseEvent*);
    QGraphicsScene* scene;
    int centerX = 5000;
    int centerY = 5000;
};

#endif // CANVAS_H
