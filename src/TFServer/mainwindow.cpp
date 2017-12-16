#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udpmanager.h"
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
    nextFrameTime(1000),
    roundCount(1),
    objects(),
    players(),
    deadplayers(),
    nextId(1)
{
    ui->setupUi(this);

    tcpmanager = new UDPManager(this);

    QString s = "Server active at:\n";
    s += tcpmanager->getAddress();
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
    map->send(tcpmanager);
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
    if(!running){

        StatusMessage start_msg = StatusMessage(StoCStatus::START);
        qDebug() << "MainWindow: StartMessage:" << &start_msg << (qint8)start_msg.status();
        *tcpmanager << &start_msg;

        map->send(tcpmanager);

        if(!started){
            qDebug() << "MainWindow: Starting game";

            started = true;
            tcpmanager->gameStarted();

            time = new QTime();
            time->start();
        }else{
            qDebug() << "MainWindow: Continuing game";

            time->restart();
        }

        ui->startButton->setText("Pause game");
        //Do one turn to init objects
        executeTurn();
        QTimer::singleShot(COUNTDOWN_TIME, this, &MainWindow::startRunning);
    }else{
        StatusMessage msg = StatusMessage(StoCStatus::PAUSED);
        qDebug() << "MainWindow: Pause message:" << &msg << (qint8)msg.status();
        *tcpmanager << &msg;
        //pause
        ui->startButton->setText("Continue game");
        running = false;
    }
}

void MainWindow::endGame(){
    qDebug() << "MainWindow: Ending the game!";

    StatusMessage msg = StatusMessage(StoCStatus::END);
    *tcpmanager << &msg;

    running = false;
    this->close();
}


void MainWindow::processUpdate(QDataStream &stream)
{
    // Read the player's id and pass the stream to it
    qint16 id;
    stream.startTransaction();
    stream >> id;
    if(stream.commitTransaction()){
        if(players.contains(id))
            players[id]->receiveData(stream);
        else
            qDebug() << "MainWindow: Incorrect player id" << id;
    }else
        qDebug() << "MainWindow: Not able to commit transaction";
}


/*
 * Adds a player to the game once connected
 * sock: the tcp socket for receiving data
 */
void MainWindow::addPlayer(qint16 id, QString name){
    player* p = new player(id, name, map, this);
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
    if(roundCount >= 5){
        endGame();
        return;
    }
    roundCount++;

    // Move dead players to active players
    for(player *p: deadplayers)
        players[p->getId()] = p;

    // Deallocate projectiles
    for(stuff *s: objects)
        if(s->getType() == Stuff::PROJECTILE)
            s->deleteLater();

    deadplayers.clear();
    objects.clear();

    // Add players to objects and resurrect them
    for(player *p: players){
        p->undie();
        objects[p->getId()] = p;
    }

    generateMap();

    QTimer::singleShot(500, this, &MainWindow::startGame);

    StatusMessage msg = StatusMessage(StoCStatus::ROUND_END);
    *tcpmanager << &msg;
}

qint16 MainWindow::getNextId(){
    return nextId++;
}

bool MainWindow::isRunning(){
    return running;
}
void MainWindow::startRunning(){
    time->restart();
    // Do one turn to empty streams from commands
    executeTurn();
    running = true;
    // Actually start the loop
    executeTurn();
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
void MainWindow::executeTurn()
{
    // Get the delta time since last frame
    int dt = time->elapsed();
    // Limit in case of lag spikes
    dt = qMin(dt, 40);
    time->restart();
    timeElapsed += dt;

    QVector<stuff*> toBeRemoved;

    // Call doStep of all objects: handles interactions & receiving commands
    for(auto object: objects)
        if(object->doStep())
            toBeRemoved += object;

    // Move objects and send their data to clients
    for(auto object: objects)
        object->move(dt, *tcpmanager);

    // Remove dead objects
    for(auto object: toBeRemoved)
        remove(object);

    updateText();

    // Re-add this to the call stack
    if(running)
        QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);

}


void MainWindow::remove(stuff *s) {
    if(s->getType() == Stuff::PLAYER){
        remove((player*)s);
    }else
        remove((projectile*)s);
}
