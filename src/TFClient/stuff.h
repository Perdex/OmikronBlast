#ifndef STUFF_H
#define STUFF_H
#include <QString>

#include <QObject>
#include <QVector>

class stuff: public QObject
{
    Q_OBJECT
public:
    stuff(double &, double &);
    ~stuff();
    void decodeType(QString);
    virtual void draw() = 0;
    double getHorizontalPos();
    double getVerticalPos();

private:
    double horizontalPos;
    double verticalPos;
};

#endif // STUFF_H
