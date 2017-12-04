#include "item.h"
#include <QString>
#include "tcpmanager.h"
#include <QDataStream>
#include "canvas.h"

Item::Item(){

}

Item::~Item(){}

void Item::read(TCPManager *manager){
    /*QDataStream* stream=manager->data;
     * QString catch;
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

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);
    painter->drawPixmap(0,0,100,100,QPixmap(":/images/Images/square.png"));
}

QRectF Item::boundingRect() const
{
    return QRectF(0,0,100,100);
}

/*int* mapping(){
    int* p;
    p=placement;
    return p;
}*/


