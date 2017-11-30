#ifndef STUFF_H
#define STUFF_H
#include <QString>

#include <QObject>
#include <QGraphicsItem>
#include <QVector>

class stuff: public QGraphicsItem
{

public:
    stuff(double &, double &);
    ~stuff();
    void decodeType(QString);
//   virtual void draw() = 0;
    double getHorizontalPos();
    double getVerticalPos();
    void setHorizontalPos(double);
    void setVerticalPos(double);
    friend QDataStream& operator>>(QDataStream&, stuff*);
private:
    double horizontalPos;
    double verticalPos;
};
#endif // STUFF_H
