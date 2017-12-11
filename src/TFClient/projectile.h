#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QtGui>

class projectile : public stuff
{
public:
    projectile(qint16 id, double &x, double &y);
    ~projectile();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

};

#endif // PROJECTILE_H
