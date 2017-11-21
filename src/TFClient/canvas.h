#ifndef CANVAS_H
#define CANVAS_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "item.h"

class Canvas : public QGraphicsView
{
    Q_OBJECT
public:
    Canvas(QWidget* p = 0);
signals:
    void keysChanged(const QMap<int, bool>&);
protected:
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    QGraphicsScene* scene;
    int centerX = 1500;
    int centerY = 1500;

    QMap<int, bool> status;
};

#endif // CANVAS_H
