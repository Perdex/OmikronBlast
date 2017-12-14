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
    QFont fontti2("AR DESTINE", 18);
    QFont fontti3("AR DESTINE", 8);

    ammo = new QGraphicsTextItem();
    fuel = new QGraphicsTextItem();
    score = new QGraphicsTextItem();
    name = new QGraphicsTextItem();
    scoreboard = new QGraphicsTextItem();

    ammo->setPos(95,43);
    ammo->setDefaultTextColor(Qt::red);
    ammo->setFont(fontti);

    fuel->setPos(95,86);
    fuel->setDefaultTextColor(Qt::red);
    fuel->setFont(fontti);

    score->setPos(95,124);
    score->setDefaultTextColor(Qt::red);
    score->setFont(fontti);

    name->setPos(22,8);
    name->setDefaultTextColor(Qt::red);
    name->setFont(fontti2);

    scoreboard->setPos(22,300);
    scoreboard->setDefaultTextColor(Qt::red);
    scoreboard->setFont(fontti3);

    scene->addItem(ammo);
    scene->addItem(fuel);
    scene->addItem(score);
    scene->addItem(name);
    scene->addItem(scoreboard);

    scene->invalidate();
}

void Infobox::setMyPlayer(player *p)
{
    my_player = p;

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
    QString n = my_player->getName();
    n.truncate(10);
    name->setPlainText(n);
}

void Infobox::update()
{
    if(my_player == nullptr) return;

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
    scoreboard->setPlainText(createScores());
}

QString Infobox::createScores()
{
    qSort(players);
    QString s = "";
    int m = qMin(10, players.size());

    for(int i = 0; i < m; i++)
    {
        QString w = players[i]->getName();
        if(w.length() > 15)
        {
            w.truncate(12);
            w.append("...");
        }
        while(w.length() < 15)
            w.append(" ");
        w.append("    ");
        w.append(QString::number(players[i]->getScore()));
        w.append("\n");
        s.append(w);
    }
    return s;
}

void Infobox::addPlayer(player *p)
{
    players.push_back(p);
}
