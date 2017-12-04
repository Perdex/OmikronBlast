#include "map.h"
#include "tcpmanager.h"
#include <QString>



Map::Map()
{
    QString str;
    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(i==0 || i==39 || j==1 || j==39){  //outlines of the cave
                map[i][j]=1;
                str.append((QString::number(map[i][j])));
            }
            else{
                map[i][j]=rand()%2;   //generates random map: 0(=no box) or 1(=box)
                str.append((QString::number(map[i][j])));   //needs some sort of id to be set apart from all data(?)
            }
        }
    }
    stream=str;
}

Map::~Map(){

}

QString Map::streaming(){
    return stream;
}

void Map::send(TCPManager* manager){
    *manager << &stream;
}

bool Map::isWall(int x, int y){
    if(this->map[x][y]==1){
        return true;
    }
    return false;
}
