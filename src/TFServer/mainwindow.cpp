#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmanager.h"
#include "player.h"
#include "stuff.h"
#include "map.h"
#include "message.h"
#include "projectile.h"

#include <QTimer>
#include <QTime>
#include <QDebug>

#define COUNTDOWN_TIME 3000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpmanager(nullptr),
    //gameLoopTimer(nullptr),
    time(nullptr),
    timeElapsed(0),
    map(nullptr),
    running(false),
    started(false),
    objects(),
    players(),
    deadplayers(),
    nextId(0)
{
    ui->setupUi(this);

    tcpmanager = new TCPManager(this);

    QString s = "Server active at:\n";
    s += tcpmanager->getAddress();
    s += "\nport: ";
    s += tcpmanager->getPort();
    ui->InfoLabel->setText(s);

    map = new Map(ui->mapView);

    // Connect the start button to start game
    QObject::connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));

    // Connect the map regenerate button
    QObject::connect(ui->regenMapButton, &QPushButton::clicked, this, &MainWindow::generateMap);
}

void MainWindow::generateMap() {
    delete map;
    map = new Map(ui->mapView);
    for(player *p: players){
        p->resetPosition(map);
        ui->mapView->addPlayer(p);
    }
    ui->mapView->updatePlayers(players.values().toVector());
}

MainWindow::~MainWindow()
{
    delete ui;
    if(tcpmanager){
        delete tcpmanager;
        tcpmanager = nullptr;
    }
    if(map){
        delete map;
        map = nullptr;
    }
    if(time){
        delete time;
        time = nullptr;
    }
    for(stuff* s: objects){
        delete s;
    }
    objects.clear();
}

/*
 * Start, pause or continue the game
 */
void MainWindow::startGame(){
    qDebug() << "Startgame called";
    if(!running){
        running = true;

        StatusMessage start_msg = StatusMessage(GameStatus::START);
        qDebug() << &start_msg;
        qDebug() << (qint8)start_msg.status();
        *tcpmanager << &start_msg;

        map->send(tcpmanager);

        if(!started){
            qDebug() << "Starting game";

            started = true;
            tcpmanager->gameStarted();

            time = new QTime();
            time->start();
        }else{
            qDebug() << "Continuing game";

            time->restart();
        }

        ui->startButton->setText("Pause game");
        nextFrameTime = COUNTDOWN_TIME;
        executeTurn();
    }else{
        StatusMessage msg = StatusMessage(GameStatus::PAUSED);
        qDebug() << &msg;
        qDebug() << (qint8)msg.status();
        *tcpmanager << &msg;
        //pause
        ui->startButton->setText("Continue game");
        running = false;
    }
}

void MainWindow::endGame(){
    qDebug() << "Ending the game!";
    running = false;
    this->deleteLater();
}


/*
 * Adds a player to the game once connected
 * sock: the tcp socket for receiving data
 */
void MainWindow::addPlayer(QDataStream *stream, qint16 id, QString name){
    player* p = new player(id, name, stream, map, this);
    objects[id] = p;
    players[id] = p;
    ui->mapView->addPlayer(p);

    QString s = "Connected players:\n";
    for(auto p: players){
        s += p->getName();
        s += '\n';
    }
    ui->PlayersLabel->setText(s);
}

void MainWindow::addProjectile(projectile *p){
    qDebug() << "Adding a projectile!!";
    objects[p->getId()] = p;
}

void MainWindow::remove(projectile *p){
    objects.remove(p->getId());
    p->deleteLater();
}
void MainWindow::remove(player *p){
    objects.remove(p->getId());
    players.remove(p->getId());
    deadplayers += p;

    // If only one player left, add new round creation to call stack
    if(players.size() <= 1)
        QTimer::singleShot(0, this, &MainWindow::newRound);

}

/*
 * Reset the game:
 * remove projectiles,
 * respawn players
 */
void MainWindow::newRound()
{
    running = false;

    // Move dead players to active players
    for(player *p: deadplayers)
        players[p->getId()] = p;

    // Deallocate projectiles
    for(stuff *s: objects)
        if(s->getType() == Stuff::PROJECTILE)
            s->deleteLater();

    deadplayers.clear();
    objects.clear();

    qDebug() << "Old objects cleared, resetting players";

    // Add players to objects and resurrect them
    for(player *p: players){
        p->undie();
        objects[p->getId()] = p;
    }

    generateMap();

    qDebug() << "Map regenerated";

    QTimer::singleShot(2000, this, &MainWindow::startGame);

    StatusMessage msg = StatusMessage(GameStatus::ROUND_END);
    *tcpmanager << &msg;
    /*
    //TODO add delay
    time->restart();
    QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);
*/
}

qint16 MainWindow::getNextId(){
    nextId++;
    return nextId;
}

bool MainWindow::isRunning(){
    return running;
}

/*
 * Updates the status text:
 * lists players in order by points, with their status
 */
void MainWindow::updateText(){

    //Set the info label text
    QString s = QString("Game running! t: %1 s").arg(timeElapsed / 1000);
    ui->InfoLabel->setText(s);

    s = "Players:\n";

    QVector<player*> pVec;
    for(player* p: players)
        pVec += p;

    ui->mapView->updatePlayers(pVec);
    //sort players by points, in descending order
    std::sort(pVec.begin(), pVec.end(), [](const player* p1, const player* p2) {
        return p1->getScore() > p2->getScore();
    });

    //add all players' info to the text
    for(player *p: pVec){
        s += p->getName() + ": ";

        if(p->getIsDead())
            s += "DEAD";

        s += QString("%1 points\n").arg(p->getScore());

    }
    ui->PlayersLabel->setText(s);
}

/*
 * Main loop content, called once every FRAME_TIME ms
 */
void MainWindow::executeTurn(){

    if(!running){
        qDebug() << "NOT doing a turn!";
        return;
    }

    int dt = time->elapsed();
    dt = qMin(dt, 50);
    time->restart();
    timeElapsed += dt;
    //qDebug() << "Doing a turn! dt: " << dt;

    QVector<stuff*> toBeRemoved;

    if(nextFrameTime != FRAME_TIME){
        qDebug() << "HERE!!!";
        qDebug() << objects.size();
        qDebug() << "idk";
    }

    for(auto object: objects)
        if(object->doStep())
            toBeRemoved += object;

    for(auto object: objects)
        object->move(dt, *tcpmanager);

    for(auto object: toBeRemoved)
        remove(object);

    updateText();

    tcpmanager->flush();
    QTimer::singleShot(nextFrameTime, this, &MainWindow::executeTurn);
    nextFrameTime = FRAME_TIME;
}


void MainWindow::remove(stuff *s) {
    objects.remove(s->getId());
    if(s->getType() == Stuff::PLAYER)
        players.remove(s->getId());
    delete s;
    // TODO Fix memory leak
}
