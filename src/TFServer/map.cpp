#include "map.h"
#include <string.h>


//idea olisi tehdä taustakuva, jonka "päälle" laatikot, kuvan koko [25][20] x32 pikseliä
Map::Map()
{

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
