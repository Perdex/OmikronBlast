#include "item.h"

Item::Item(){}

QRectF Item::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    QLineF line(10.0, 80.0, 90.0, 20.0);
     painter->drawLine(line);
}

