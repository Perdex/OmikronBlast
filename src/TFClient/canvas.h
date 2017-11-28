#ifndef CANVAS_H
#define CANVAS_H

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
    void addPlayer(player*);
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
    //int centerX = 1500;
    //int centerY = 1500;

    QMap<int, bool> status;

    player *my_player;

    bool mouseKey1Down;
};

#endif // CANVAS_H
