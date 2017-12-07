#include "canvas.h"
#include "player.h"
#include "item.h"
#include <QtDebug>

Canvas::Canvas(QWidget* p) :
    QGraphicsView(p), status(), my_player(nullptr), mouseKey1Down(false), map()
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
    mouseX = p->x();
    mouseY = p->y();
    qDebug() << mouseY;
    center();
}

void Canvas::addPlayer(player *p) {
    scene->addItem(p);
}

void Canvas::addItem(Item* item, int x, int y) {
    scene->addItem(item);
    item->setPos(x, y);
}

void Canvas::buildMap(QString s) {
    map = new QPixmap(scene->width(), scene->height());
    map->fill(Qt::transparent);
    QPixmap tile(":/images/Squarebox.png");
    //tile = tile.scaled(30, 30);
    QPainter painter(map);

    //Tämäkin on testi
    //Tähän pitäisi laittaa koko kartan piirtäminen
    //merkkijonosta s.
    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(s[40 * i + j]=='1'){
                painter.drawPixmap(i*100, j*100, tile);
            }
        }
    }

    scene->addPixmap(*map);
}

void Canvas::buildMap(int p[][39]){
    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(p[i][j]==1){
                Item* it= new Item();
                scene->addItem(it);
                it->setPos(i*100, j*100);
            }
        }
    }
}

void Canvas::center() {
    if(my_player == nullptr) return;
    qDebug() << my_player->getHorizontalPos() + (mouseX - this->width()/2)/2;
    this->centerOn(my_player->getHorizontalPos() + (mouseX - this->width()/2)/2, my_player->getVerticalPos() + (mouseY - this->height()/2)/2);
}

void Canvas::mouseMoveEvent(QMouseEvent *me) {
    if(my_player == nullptr) return;
    mouseY = me->y();
    mouseX = me->x();
    center();
    //this->centerOn(my_player->getHorizontalPos() + (me->x() - this->width()/2)/2, my_player->getVerticalPos() + (me->y() - this->height()/2)/2);
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

bool Canvas::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == viewport() && ev->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}


