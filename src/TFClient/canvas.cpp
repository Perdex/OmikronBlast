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

// t.jaakko
    double x = 2500.0;
    double y = 2500.0;
    player *dude = new player("pertti",'f',x, y);
    dude->setPos(dude->getHorizontalPos(),dude->getVerticalPos());
    scene->addItem(dude);
    this->centerOn(dude->getHorizontalPos(),dude->getVerticalPos());


//    this->centerOn(centerX,centerY);

    scene->invalidate();
}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    double x = 2500.0;
    double y = 2500.0;
    player *dude = new player("pertti",'f',x, y);
    this->centerOn(dude->getHorizontalPos() + (me->x() - this->width()/2)/2, dude->getVerticalPos() + (me->y() - this->height()/2)/2);
    // ^^varmaan pitää hakee jostain playerlistasta mut en osannu tehä tähän hätään muutenkaan

//    this->centerOn(centerX + (me->x() - this->width()/2)/2, centerY + (me->y() - this->height()/2)/2);
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


