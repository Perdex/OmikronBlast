#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "stuff.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QtGui>

class UpdateMessage;

class projectile : public stuff
{
public:
    projectile(qint16 id, double &x, double &y, double angle);
    ~projectile();
    static stuff* create(qint16, QDataStream *);
    void update(QDataStream *);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
private:
    double angle;
    QPixmap pixmap;
};

#endif // PROJECTILE_H
