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

        for(int i = 1; i < W-1; i++) {
            for(int j = 1; j < H-1; j++) {
                int count1 = 0, count2 = 0;
                for(int dx = -1; dx <= 1; dx++) {
                    for(int dy = -1; dy <= 1; dy++) {
                        count1 += cave[(i+dx)*H+(j+dy)];
                    }
                }

                for(int dx = -2; dx <= 2; dx++) {
                    for(int dy = -2; dy <= 2; dy++) {
                        count2 += cave[(qBound(0, i+dx, W-1))*H+(qBound(0, j+dy, H-1))];
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
            stream += cave[i*H+j] ? '1' : '0';
            map[i][j] = cave[i*H+j];
        }
    }

    // Set preview

    mp->setMap(cave);

    delete[] cave;
}


void Map::send(TCPManager* manager){
    StatusMessage msg = StatusMessage(GameStatus::MAP_TRANSFER, stream);
    *manager << &msg;
}

bool Map::isWall(int x, int y){
    return this->map[x][y] == 1;
}

bool Map::touches(double x, double y){
    return this->map[(int)x / 100][(int)y / 100] == 1;
}

int signum(int i){
    return i > 0 ? 1 : i < 0 ? -1 : 0;
}
bool Map::collide(double *x, double *y, double *vx, double *vy, int dt, double bounce){

    int prev_x = (int)*x / 100;
    int prev_y = (int)*y / 100;

    int next_x = (int)(*x + *vx * dt) / 100;
    int next_y = (int)(*y + *vy * dt) / 100;

    // The difference in position
    int dx = next_x - prev_x;
    int dy = next_y - prev_y;

    // Stays within the same block: can return
    if(dx == 0 && dy == 0)
        return false;

    // If not colliding, or is for some reason in a block, return
    if(map[next_x][next_y] == 0)// || map[prev_x][prev_y] != 0)
        return false;

    // Collision is happening: determine the direction
    // and store the (anti-)normal direction to n_x, n_y
    double n_x = signum(dx);
    double n_y = signum(dy);

    // Handle diagonal movement
    if(dx != 0 && dy != 0){
        int a = map[next_x][prev_y];
        int b = map[prev_x][next_y];
        if(a == 1 && b == 0) // Vertical wall
            n_y = 0;
        else if(a == 0 && b == 1) // Horizontal wall
            n_x = 0;
        else{ // Corner
            double s = 1. / sqrt(2);
            n_x *= s;
            n_y *= s;
        }
    }

    // Dot product of the wall and velocity
    double dot = *vx * n_x + *vy * n_y;

    bounce += 1;

    // reflect the velocity
    *vx -= bounce * dot * n_x;
    *vy -= bounce * dot * n_y;

    // set the position to edge of wall
    if(n_x != 0)
        *x = round((*x + *vx * dt * 0.5) / 100) * 100 - n_x / 10;
    if(n_y != 0)
        *y = round((*y + *vy * dt * 0.5) / 100) * 100 - n_y / 10;

    return true;
}

