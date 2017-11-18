#ifndef MAP_H
#define MAP_H


class Map
{

public:
    Map();
    ~Map();
    string streaming();
    void isWall;

private:
    string stream;
    int map[30][25];
};

#endif // MAP_H
