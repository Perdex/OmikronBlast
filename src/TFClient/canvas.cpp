#include "canvas.h"
#include "player.h"
#include "projectile.h"
#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p), status(), my_player(nullptr), mouseKey1Down(false), map(nullptr)
{   
    viewport()->installEventFilter(this);

    status[Qt::Key_W] = false;
    status[Qt::Key_A] = false;
    status[Qt::Key_S] = false;
    status[Qt::Key_D] = false;
    status[Qt::Key_Space] = false;

    scene = new QGraphicsScene(0,0,4000,4000, this);
    this->setScene(scene);
    QPixmap pm(":/images/Images/background_GMaps.png");
    pm = pm.scaled(scene->width(), scene->height());
    background = new QGraphicsPixmapItem();
    background->setPixmap(pm);
    scene->addItem(background);

    this->scale(0.5, 0.5);
}

void Canvas::setMyPlayer(player* p) {
    my_player = p;
    if(my_player != nullptr) {
        mouseX = p->x();
        mouseY = p->y();
        center();
    }
}

void Canvas::addStuff(stuff *p) {
    scene->addItem(p);
}

void Canvas::buildMap(QString s) {
    QPixmap pm(scene->width(), scene->height());
    pm.fill(Qt::transparent);
    QPixmap tile(":/images/Squarebox.png");
    tile = tile.scaled(100, 100);
    QPainter painter(&pm);

    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(s[40 * i + j]=='1'){
                painter.drawPixmap(i*100, j*100, tile);
            }
        }
    }

    scene->removeItem(map);
    delete map;
    map = scene->addPixmap(pm);
}

void Canvas::center() {
    if(my_player == nullptr) return;

    QPoint sceneCenter(scene->width()/2, scene->height()/2);

    if(!my_player->getDead()) {
        QPoint viewCenterCand(my_player->getHorizontalPos() + (mouseX - this->width()/2)/2,
                          my_player->getVerticalPos() + (mouseY - this->height()/2)/2);

        this->centerOn(viewCenterCand);
    }

    QPointF viewCenter = mapToScene(viewport()->rect().center());

    QPointF shift = 0.5*(viewCenter - sceneCenter);

    background->setPos(shift);

}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    if(my_player == nullptr) return;

    mouseY = me->y();
    mouseX = me->x();

    /*if(my_player->getDead()) {
        QPoint c = this->viewport()->rect().center();
        QPoint shift = c - me->pos();
        if(shift.manhattanLength() > 100) {
            this->centerOn(mapToScene(c) - shift/8);
            center();
        }
        return;
    }*/
    center();
    QPointF p = mapToScene(me->x(), me->y());
    my_player->setAngle(atan2(p.y() - my_player->getVerticalPos(), p.x() - my_player->getHorizontalPos()) * 180 / M_PI);
}

void Canvas::mousePressEvent(QMouseEvent *me) {
    if(my_player == nullptr) return;
    if(!mouseKey1Down && me->button() == Qt::LeftButton) {
        mouseKey1Down = true;
        QPointF p = mapToScene(me->pos());
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
    if(my_player != nullptr && my_player->getDead()) {
        int k = ke->key();

        int dx = 0;
        if(k == Qt::Key_A) dx = -1;
        else if(k == Qt::Key_D) dx = 1;

        int dy = 0;
        if(k == Qt::Key_W) dy = -1;
        else if(k == Qt::Key_S) dy = 1;

        this->centerOn(mapToScene(this->viewport()->rect().center() + 100*QPoint(dx, dy)));
        center();
        return;
    }
    if(!ke->isAutoRepeat() && status.contains(ke->key()) && !status[ke->key()]) {
        status[ke->key()] = true;
        emit statusChanged(status, 0, false);
    }
}

void Canvas::keyReleaseEvent(QKeyEvent *ke)
{
    if(!ke->isAutoRepeat() && status.contains(ke->key()) && status[ke->key()]) {
        status[ke->key()] = false;
        emit statusChanged(status, 0, false);
        center();
    }
}

void Canvas::remove(stuff *s) {
    scene->removeItem(s);
}

bool Canvas::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == viewport() && ev->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}


