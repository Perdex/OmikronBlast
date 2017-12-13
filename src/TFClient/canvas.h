#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "item.h"
#include "stuff.h"
#include "player.h"

#define ANGLE 0

class Canvas : public QGraphicsView
{
    Q_OBJECT
public:
    Canvas(QWidget* p = 0);
    void setMyPlayer(player*);
    void addStuff(stuff*);
    void addItem(Item*, int, int);
    void buildMap(int [][39]);
    void buildMap(QString);
    void center();
    void remove(stuff*);
signals:
    void statusChanged(const QMap<int, bool>&, float ang, bool clicked);
protected:
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool eventFilter(QObject *, QEvent *);

    QGraphicsScene* scene;

    QMap<int, bool> status;
    double mouseX, mouseY;

    player *my_player;

    bool mouseKey1Down;

    QPixmap *map;
    QGraphicsPixmapItem *background;
};

#endif // CANVAS_H
