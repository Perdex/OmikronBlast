#ifndef STUFF_H
#define STUFF_H

#include <QObject>
#include <QVector>

class stuff: public QObject
{
    Q_OBJECT
public:
    stuff(double &, double &, double, double);
    ~stuff();
    double getVerticalPos() const;
    double getHorizontalPos() const;
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    virtual void move() = 0;
    void changeVerticalSpeed(double);
    void changeHorizontalSpeed(double);
    void setVerticalSpeed(double);
    void setHorizontalSpeed(double);
    void changeVerticalPos(double);
    void changeHorizontalPos(double);

private:
    double horizontalPos;
    double verticalPos;
    double horizontalSpeed;
    double verticalSpeed;
};

#endif // STUFF_H
