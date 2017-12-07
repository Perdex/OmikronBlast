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
                int first=rand()%2;
                if(first==1){
                map[i][j]=rand()%2;   //generates random map: 0(=no box) or 1(=box)
                }
                else{
                    map[i][j]=first;
                }
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

std::pair<int, int> Map::newPoint(int x, int y, std::pair<int, int> vector){
    int true_x=x;
    int true_y=y;

    int x_move=vector.first;
    int y_move=vector.second;

    int new_x=true_x+x_move;
    int new_y=true_y+y_move;

    std::string coord_x=std::to_string(new_x);
    std::string coord_y=std::to_string(new_y);

    int len_x=coord_x.length();
    int len_y=coord_y.length();

    int i,j;

    //seuraavat if elset tod.näk.turhia
    if(coord_x[0]=='1'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='2'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='3'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='4'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='5'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='6'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='7'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='8'){
        i=coord_x[0] - '0';
    }
    else if(coord_x[0]=='9'){
        i=coord_x[0] - '0';
    }

    if(coord_y[0]=='1'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='2'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='3'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='4'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='5'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='6'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='7'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='8'){
        j=coord_y[0] - '0';
    }
    else if(coord_y[0]=='9'){
        j=coord_y[0] - '0';
    }

    int left_x=(new_x - (new_x % 100))/100;
    int left_y=(new_y - (new_y % 100))/100;

    if(this->map[left_y][left_x]==1){
        //pitäisi palauttaa osumakohta laattaan, miten?!
    }
    else{
        return std::make_pair(new_x, new_y);
    }

}

