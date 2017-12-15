#ifndef STUFF_H
#define STUFF_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QtGlobal>

class TCPManager;
class Map;
class MainWindow;

enum class Stuff : qint8 {
    PLAYER, PROJECTILE
};

class stuff: public QObject
{
    Q_OBJECT
public:
    stuff(Stuff t, qint16 id, Map *map, MainWindow *main, QDataStream *s,
          int x = 2500, int y = 2500);
    ~stuff();
    qint16 getId() const;
    bool getIsDead() const;
    double getVerticalPos() const;
    double getHorizontalPos() const;
    double getVerticalSpeed() const;
    double getHorizontalSpeed() const;
    virtual bool doStep() = 0;
    virtual void move(int dt, TCPManager& mgr) = 0;
    Stuff getType() const { return type; }
    friend QDataStream& operator<<(QDataStream& stream, const stuff &s);
protected:
    qint16 id;
    double x;
    double y;
    double vx;
    double vy;
    bool isDead;
    Stuff type;
    Map* map;
    MainWindow *mainWindow;
protected:
    QDataStream *stream;
};

#endif // STUFF_H
