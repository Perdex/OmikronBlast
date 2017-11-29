#ifndef STUFF_H
#define STUFF_H

#include <QObject>
#include <QVector>

class TCPManager;

class stuff: public QObject
{
    Q_OBJECT
public:
    stuff(double & x, double & y , double dx, double dy);
    stuff(QDataStream *s){}//TODO implement this
    ~stuff();
    virtual QDataStream& operator<<(QDataStream& stream) = 0;
    double getVerticalPos() const;
    double getHorizontalPos() const;
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    virtual void move(int dt, TCPManager* mgr) = 0;
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
