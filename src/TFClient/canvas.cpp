#include "canvas.h"

#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p),
    status()
{
    viewport()->installEventFilter(this);

    status[Qt::Key_W] = false;
    status[Qt::Key_A] = false;
    status[Qt::Key_S] = false;
    status[Qt::Key_D] = false;
    status[Qt::Key_Space] = false;

    scene = new QGraphicsScene(0,0,4000,4000, this);
    this->setScene(scene);
    QPixmap pm(":/images/Images/background.png");
    pm = pm.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    item->setPixmap(pm);
    scene->addItem(item);


    double x = 1000.0;
    double y = 1000.0;
    player *dude = new player("pertti",'f',x, y);
    dude->setPos(dude->getHorizontalPos(),dude->getVerticalPos());
    scene->addItem(dude);


    this->centerOn(centerX, centerY);

    scene->invalidate();
}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    this->centerOn(centerX + (me->x() - this->width()/2)/2, centerY + (me->y() - this->height()/2)/2);
}

void Canvas::keyPressEvent(QKeyEvent *ke)
{
    if(status.contains(ke->key()) && !status[ke->key()]) {
        status[ke->key()] = true;
        emit keysChanged(status);
    }
}

void Canvas::keyReleaseEvent(QKeyEvent *ke)
{
    if(status.contains(ke->key()) && status[ke->key()]) {
        status[ke->key()] = false;
        emit keysChanged(status);
    }
}

bool Canvas::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == viewport() && ev->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}


