#include "item.h"
#include <QString>
#include "tcpmanager.h"
#include <QDataStream>
#include "canvas.h"

Item::Item(){}

Item::~Item(){}

void Item::read(TCPManager *manager){
    /*QDataStream* stream=manager->data;
     * QString=catch;
     * stream >> catch;
     * int k=0;
     * for(int i=0; i<20; i++){
     *  for(int j=0; j<25; j++){
     *      placement[i][j]=catch[k].toInt();
     *      k++;
     *  }
     * }
     */
}

void Item::draw(){
    QImage block(":/images/Images/squarebox.png");
    QGraphicsPixmapItem item( QPixmap::fromImage(block));
    for(int i=0; i<20; i++){
        for(int j=0; j<25; j++){
            if(placement[i][j]==1){
                //put the item on canvas to place (j,i)=(x,y)
            }
        }
    }
}

