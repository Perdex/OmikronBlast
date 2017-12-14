#ifndef MAPPREVIEW_H
#define MAPPREVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>

class QGraphicsScene;
class player;

class MapPreview : public QGraphicsView
{
public:
    MapPreview(QWidget *parent = nullptr);
    ~MapPreview();
    void setMap(bool*);
    void addPlayer(player *p);
    void updatePlayers(QVector<player*> &v);
private:
    QGraphicsScene scene;
    const int W = 40, H = 40;
    QMap<qint16, QGraphicsItem*> playerItems;
};

#endif // MAPPREVIEW_H
