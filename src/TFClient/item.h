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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    //int* mapping();

private:
    int placement[39][39];
};

#endif // ITEM_H
