#include "mappreview.h"

#include <QGraphicsScene>
#include <QtDebug>

MapPreview::MapPreview(QWidget *parent)
    : QGraphicsView(parent), scene(0, 0, 240, 240, this)
{
    this->setScene(&scene);
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
