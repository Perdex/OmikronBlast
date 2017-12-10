#ifndef MAP_H
#define MAP_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QString>
#include <utility>
#include <string>
#include <math.h>

class TCPManager;
class MapPreview;

class Map
{

public:
    Map(MapPreview*);
    ~Map(); //turha?
    QString streaming();
    bool isWall(int x, int y);
    void send(TCPManager* manager);
    // if necessary, collides with a wall and updates the position and velocity
    void collide(double *x, double *y, double *vx, double *vy, int dt);

private:
    QString stream;
    int map[40][40];
};

#endif // MAP_H
