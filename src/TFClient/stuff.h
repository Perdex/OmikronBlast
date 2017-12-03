#ifndef STUFF_H
#define STUFF_H
#include <QString>

#include <QObject>
#include <QGraphicsItem>
#include <QVector>

class stuff: public QGraphicsItem
{

public:
    stuff(qint16 id, double &x, double &y);
    ~stuff();
    void decodeType(QString);
//   virtual void draw() = 0;
    double getHorizontalPos();
    double getVerticalPos();
    void setHorizontalPos(double);
    void setVerticalPos(double);
    friend QDataStream& operator>>(QDataStream&, stuff*);
private:
    qint16 id;
    double horizontalPos;
    double verticalPos;
};
#endif // STUFF_H
