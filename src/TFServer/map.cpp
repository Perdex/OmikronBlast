#include "map.h"
#include <QString>


//idea olisi tehdä taustakuva, jonka "päälle" laatikot, kuvan koko [20][25] x32 pikseliä
Map::Map()
{
    QString str;
    for(int i=0; i<20; i++){
        for(int j=0; j<25; j++){
            map[i][j]=rand()%2;   //generates random map: 0(=no box) or 1(=box)
            str.append((QString::number(map[i][j])));
        }
    }
    stream=str;
}

QString Map::streaming(){
    return stream;
}

bool Map::isWall(int x, int y){
    if(this->map[x][y]==1){
        return true;
    }
    return false;
}
