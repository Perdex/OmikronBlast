#ifndef MAP_H
#define MAP_H
#include <QString>

class Map
{

public:
    Map();
    ~Map();
    QString streaming(); //turha?
    bool isWall(int x, int y);

private:
    QString stream;  //turha?
    int map[25][20];
};

#endif // MAP_H
