#include "infobox.h"

#include <QTimer>

Infobox::Infobox(QWidget* p) : QGraphicsView(p), countdown(), my_player(nullptr)
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
    QFont fontti3("AR DESTINE", 10);

    ammo = new QGraphicsTextItem();
    fuel = new QGraphicsTextItem();
    score = new QGraphicsTextItem();
    name = new QGraphicsTextItem();
    scoreboard = new QGraphicsTextItem();
    countdown = new QGraphicsTextItem();

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

    countdown->setPos(22, 200);
    countdown->setFont(fontti2);
    countdown->setDefaultTextColor(Qt::red);
    countdown->setPlainText("Waiting...");

    scene->addItem(ammo);
    scene->addItem(fuel);
    scene->addItem(score);
    scene->addItem(name);
    scene->addItem(scoreboard);
    scene->addItem(countdown);

    scene->invalidate();
}

void Infobox::countDown(int sec) {
    if(sec == 0) {
        countdown->setPlainText("Game ON!");
    }
    else if(sec == -1) {
        countdown->setPlainText("Game paused");
    } else if(sec == -2) {
        countdown->setPlainText("Game ended");
    }else{
        countdown->setPlainText(QString("Starts in %1").arg(sec));
        QTimer::singleShot(1000, this, [this, sec]{ this->countDown(sec-1); });
    }
}

void Infobox::setMyPlayer(player *p)
{
    my_player = p;

    if(my_player == nullptr) return;

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
    QString n = my_player->getName();
    n.truncate(10);
    name->setPlainText(n);
}

void Infobox::update(QMap<qint16, stuff*> items)
{
    if(my_player == nullptr) return;

    if(my_player->getDead()) {
        countdown->setPlainText("DEAD!!");
        return;
    }

    ammo->setPlainText(QString::number(my_player->getAmmo()));
    fuel->setPlainText(QString::number(my_player->getFuel()));
    score->setPlainText(QString::number(my_player->getScore()));
    scoreboard->setPlainText(createScores(items));
}

QString Infobox::createScores(QMap<qint16, stuff*> items)
{
    QVector <player*> players;

    for(QMap<qint16, stuff*>::iterator it = items.begin(); it != items.end(); it++)
    {
        if((*it)->getType() == Stuff::PLAYER)
            players.push_back(static_cast<player*>(*it));
    }
    std::sort(players.begin(), players.end());
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
