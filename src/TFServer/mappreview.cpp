#include "mappreview.h"
#include "player.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QDebug>

MapPreview::MapPreview(QWidget *parent)
    : QGraphicsView(parent),
      scene(0, 0, 240, 240, this),
      playerItems()
{
    this->setScene(&scene);
}

MapPreview::~MapPreview(){
    for(QGraphicsItem *i: playerItems){
        delete i;
    }
    playerItems.clear();
}


void MapPreview::addPlayer(player *p){
    auto item = new QGraphicsEllipseItem(0, 0, 6, 6);
    item->setBrush(Qt::red);
    scene.addItem(item);
    playerItems[p->getId()] = item;
}

void MapPreview::updatePlayers(QVector<player*> &v){
    for(player *p: v){
        auto i = playerItems[p->getId()];
        double x = p->getHorizontalPos() * scene.width() / 4000;
        double y = p->getVerticalPos() * scene.height() / 4000;
        i->setX(x-3);
        i->setY(y-3);
    }
}

void MapPreview::setMap(bool *map) {
    double tileWidth = scene.width()/(double)W;
    double tileHeight = scene.height()/(double)H;

    scene.clear();

    QPixmap buffer(scene.width(),scene.height());
    buffer.fill(Qt::transparent);
    QPainter painter(&buffer);

    for(int i = 0; i < W; i++) {
        for(int j = 0; j < H; j++) {
            if(map[i*H+j]) {
                painter.fillRect(
                            i*tileWidth, j*tileHeight,
                            tileWidth, tileHeight, Qt::black);
            }
        }
    }
    scene.addPixmap(buffer);
}
