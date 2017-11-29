#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QPainter>

class Canvas;
class TCPManager;
class Item : public QGraphicsItem
{
public:
    Item();
    ~Item();
    void read(TCPManager *manager);
    void draw(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    int placement[25][20];
};

#endif // ITEM_H
