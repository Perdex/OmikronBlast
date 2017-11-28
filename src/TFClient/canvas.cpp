#include "canvas.h"
#include "player.h"
#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p), status()
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

    scene->invalidate();
}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    this->centerOn(my_player->getHorizontalPos() + (me->x() - this->width()/2)/2, my_player->getVerticalPos() + (me->y() - this->height()/2)/2);
}

void Canvas::keyPressEvent(QKeyEvent *ke)
{
    if(status.contains(ke->key()) && !status[ke->key()]) {
        status[ke->key()] = true;
        emit keysChanged(status);
    }
}

void Canvas::setMyPlayer(player* p) {
    my_player = p;
    this->centerOn(p->x(), p->y());
}

void Canvas::addPlayer(player *p) {
    scene->addItem(p);
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


