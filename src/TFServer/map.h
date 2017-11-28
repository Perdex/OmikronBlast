#ifndef MAP_H
#define MAP_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QString>

class TCPManager;

class Map
{

public:
    Map();
    ~Map();
    QString streaming();
    bool isWall(int x, int y);
    void send(TCPManager* manager);

private:
    QString stream;
    int map[20][25];
};

#endif // MAP_H
