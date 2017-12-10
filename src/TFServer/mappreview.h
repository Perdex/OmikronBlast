#ifndef MAPPREVIEW_H
#define MAPPREVIEW_H

#include <QGraphicsView>

class QGraphicsScene;

class MapPreview : public QGraphicsView
{
public:
    MapPreview(QWidget *parent = nullptr);
    void setMap(bool*);
private:
    QGraphicsScene scene;
    const int W = 40, H = 40;
};

#endif // MAPPREVIEW_H
