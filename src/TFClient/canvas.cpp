#include "canvas.h"
#include "player.h"
#include "item.h"
#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p), status(), mouseKey1Down(false)
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

void Canvas::setMyPlayer(player* p) {
    my_player = p;
    this->centerOn(p->x(), p->y());
}

void Canvas::addPlayer(player *p) {
    scene->addItem(p);
}

void Canvas::addItem(Item* item, int x, int y) {
    scene->addItem(item);
    item->setPos(x, y);
}


void Canvas::mouseMoveEvent(QMouseEvent *me) {
    this->centerOn(my_player->getHorizontalPos() + (me->x() - this->width()/2)/2, my_player->getVerticalPos() + (me->y() - this->height()/2)/2);
}

void Canvas::mousePressEvent(QMouseEvent *me) {
    if(!mouseKey1Down && me->button() == Qt::LeftButton) {
        mouseKey1Down = true;
        QPointF p = mapToScene(me->pos());
        //qDebug() << p;
        emit statusChanged(status, qAtan2(p.y() - my_player->y(), p.x() - my_player->x()), true);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mouseKey1Down = false;
    }
}

void Canvas::keyPressEvent(QKeyEvent *ke)
{
    if(status.contains(ke->key()) && !status[ke->key()]) {
        status[ke->key()] = true;
        emit statusChanged(status, 0, false);
    }
}

void Canvas::keyReleaseEvent(QKeyEvent *ke)
{
    if(status.contains(ke->key()) && status[ke->key()]) {
        status[ke->key()] = false;
        emit statusChanged(status, 0, false);
    }
}

bool Canvas::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == viewport() && ev->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}


