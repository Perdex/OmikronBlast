#ifndef STUFF_H
#define STUFF_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QtGlobal>

class TCPManager;

class stuff: public QObject
{
    Q_OBJECT
public:
    stuff(qint16 id, double & x, double & y , double dx, double dy);
    stuff(qint16 id, QDataStream *s);
    ~stuff();
    //virtual QDataStream& operator<<(QDataStream& stream) = 0;
    qint16 getId() const;
    double getVerticalPos() const;
    double getHorizontalPos() const;
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    virtual void doStep(int dt) = 0;
    virtual void move(int dt, TCPManager& mgr) = 0;
    void changeVerticalSpeed(double);
    void changeHorizontalSpeed(double);
    void setVerticalSpeed(double);
    void setHorizontalSpeed(double);
    void changeVerticalPos(double);
    void changeHorizontalPos(double);
    void setVerticalPos(double);
    void setHorizontalPos(double);
    friend QDataStream& operator<<(QDataStream& stream, const stuff &s);
private:
    qint16 id;
    double horizontalPos;
    double verticalPos;
    double horizontalSpeed;
    double verticalSpeed;
protected:
    QDataStream *stream;
};

#endif // STUFF_H
