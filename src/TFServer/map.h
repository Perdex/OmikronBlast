#ifndef MAP_H
#define MAP_H


class Map
{

public:
    Map();
    ~Map();
    string streaming(); //turha?
    void isWall;

private:
    string stream;  //turha?
    int map[30][30];
};

#endif // MAP_H
