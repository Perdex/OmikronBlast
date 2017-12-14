#include "infobox.h"

Infobox::Infobox(QWidget* p) : QGraphicsView(p), my_player(nullptr)
{
    scene = new QGraphicsScene(0,0,200,682, this);
    this->setScene(scene);
    QPixmap pm(":/images/Images/infoback.png");
    pm = pm.scaled(scene->width(), scene->height());
    background = new QGraphicsPixmapItem();
    background->setPixmap(pm);
    scene->addItem(background);

    QFont fontti("AR DESTINE", 14);

    ammo = new QGraphicsTextItem();
    fuel = new QGraphicsTextItem();
    score = new QGraphicsTextItem();

    ammo->setPos(95,43);
    ammo->setDefaultTextColor(Qt::red);
    ammo->setFont(fontti);

    fuel->setPos(95,86);
    fuel->setDefaultTextColor(Qt::red);
    fuel->setFont(fontti);

    score->setPos(95,124);
    score->setDefaultTextColor(Qt::red);
    score->setFont(fontti);

    scene->addItem(ammo);
    scene->addItem(fuel);
    scene->addItem(score);

    scene->invalidate();
}

void Infobox::setMyPlayer(player *p)
{
    my_player = p;

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
}

void Infobox::update()
{
    if(my_player == nullptr) return;

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
}
