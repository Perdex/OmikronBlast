#ifndef MAP_H
#define MAP_H
#include <QString>

class Map
{

public:
    Map();
    ~Map();
    QString streaming();
    bool isWall(int x, int y);

private:
    QString stream;
    int map[20][25];
};

#endif // MAP_H
