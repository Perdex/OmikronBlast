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
    std::pair<int, int> newPoint(int, int, std::pair<int, int>);

private:
    QString stream;
    int map[40][40];
};

#endif // MAP_H
