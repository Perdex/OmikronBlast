#include "map.h"
#include "tcpmanager.h"
#include "message.h"
#include "mappreview.h"

#include <QString>
#include <QQueue>
#include <QSet>
#include <QtMath>

Map::Map(MapPreview *mp)
{   
    const int W = 40, H = 40;

    time_t t; time(&t); srand((unsigned int) t);

    int initialWallPropability = 35;

    bool *cave = new bool[W*H];

    // Generate initial state for cellular automaton

    for(int i = 0; i < W; i++) {
        for(int j = 0; j < H; j++) {
            cave[i*H+j] = i == 0 || j == 0 || i == W-1 || j == H-1 || rand() % 100 < initialWallPropability;
        }
    }

    // Run cellular automaton 5 times

    for(int c = 0; c < 5; c++) {
        bool *ncave = new bool[W*H];

        for(int i = 0; i < W; i++) {
            ncave[i*H] = true;
            ncave[(i+1)*H-1] = true;
        }

        for(int i = 0; i < H; i++) {
            ncave[i] = true;
            ncave[H*(W-1)+i] = true;
        }

        for(int i =1; i < W-1; i++) {
            for(int j = 1; j < H-1; j++) {
                int count1 = 0, count2 = 0;
                for(int dx = -1; dx <= 1; dx++) {
                    for(int dy = -1; dy <= 1; dy++) {
                        count1 += cave[(i+dx)*H+(j+dy)];
                    }
                }

                for(int dx = -2; dx <= 2; dx++) {
                    for(int dy = -2; dy <= 2; dy++) {
                        count2 += cave[(i+dx)*H+(j+dy)];
                    }
                }

                ncave[i*H+j] = count1 >= 5 || count2 <= 2;
            }
        }
        delete[] cave;
        cave = ncave;
    }

    // Simple flood fill connectivity test

    QVector<QSet<int>> areas;
    bool visited[W*H];

    for(int i = 0; i < W*H; i++)
        visited[i] = cave[i];

    for(int i = 1; i < W-1; i++) {
        for(int j = 1; j < H-1; j++) {
            if(!visited[i*H+j]) {
                QSet<int> newArea;

                QQueue<int> queue;
                queue.enqueue(i*H+j);
                visited[i*H+j] = true;

                while(!queue.empty()) {
                    int pos = queue.dequeue();

                    newArea.insert(pos);

                    int npos[] = {
                        pos + 1,
                        pos - 1,
                        pos + H,
                        pos - H
                    };

                    for(int p: npos) {
                        if(!visited[p]) {
                            queue.enqueue(p);
                            visited[p] = true;
                        }
                    }
                }

                areas.append(newArea);
            }
        }
    }

    // Ensure connectivity by doing random walk from every area
    // to the largest area.

    if(areas.size() > 1) {
        QSet<int> largest = areas[0];
        int maxIdx = 0;
        for(int i = 1; i < areas.size(); i++) {
            if(areas[i].count() > largest.count()) {
                largest = areas[i];
                maxIdx = i;
            }
        }

        for(int i = 0; i < areas.size(); i++) {
            if(i == maxIdx) continue;

            QSet<int> area = areas[i];

            int pos = area.values()[qrand() % area.size()];
            int t = largest.values()[qrand() % largest.size()];
            QPoint target(t/H, t%H);

            while(largest.find(pos) == largest.end()) {
                QPoint ppos(pos/H, pos%H);

                QPointF eps = 3*QPoint((qrand()%5) - 2, (qrand()%5) - 2);
                QPointF exp = target - ppos;
                exp /= qSqrt(QPointF::dotProduct(exp,exp));

                QPointF delta = exp + eps;


                if(qAbs(delta.x()) < qAbs(delta.y())) {
                    ppos += QPoint(0, delta.y()>0 ? 1:-1);
                }else{
                    ppos += QPoint(delta.x()>0 ? 1:-1, 0);
                }

                pos = qBound(1, ppos.x(), W-2)*H + qBound(1, ppos.y(), H-2);
                cave[pos] = false;
                area.insert(pos);
            }

            for(int p: area.values()) {
                largest.insert(p);
            }
        }
    }

    // Serialize the map

    stream = "";

    for(int i = 0; i < W; i++) {
        for(int j = 0; j < H; j++) {
            stream += cave[i*H+j] ? "1" : "0";
            map[i][j] = cave[i*H+j];
        }
    }

    // Set preview

    mp->setMap(cave);

    delete[] cave;
}

Map::~Map(){

}

QString Map::streaming(){
    return stream;
}

void Map::send(TCPManager* manager){

    StatusMessage *msg = new StatusMessage(GameStatus::MAP_TRANSFER, stream);
    *manager << msg;
}

bool Map::isWall(int x, int y){
    return this->map[x][y] == 1;
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

