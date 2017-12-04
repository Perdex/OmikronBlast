#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmanager.h"
#include "player.h"
#include "stuff.h"
#include "map.h"

#include <QTimer>
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpmanager(nullptr),
    gameLoopTimer(nullptr),
    time(nullptr),
    timeElapsed(0),
    map(nullptr),
    running(false)
{
    ui->setupUi(this);

    tcpmanager = new TCPManager(this);

    QString s = "Server active at:\n";
    s += tcpmanager->getAddress();
    s += "\nport: ";
    s += tcpmanager->getPort();
    ui->InfoLabel->setText(s);

    //initialize and connect the main loop timer: will not run yet
    gameLoopTimer = new QTimer(this);
    QObject::connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(executeTurn()));

    //connect the start button to start game
    QObject::connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
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

void MainWindow::startGame(){
    qDebug() << "Starting game";

    running = true;

    tcpmanager->gameStarted();

    map = new Map();
    map->send(tcpmanager);

    gameLoopTimer->start(FRAME_TIME);
    time = new QTime();
    time->start();
}

void MainWindow::endGame(){
    qDebug() << "Ending the game!";
    running = false;
    gameLoopTimer->stop();
}


/*
 * Adds a player to the game once connected
 * sock: the tcp socket for receiving data
 */
void MainWindow::addPlayer(qint16 id, QDataStream *stream){
    player* p = new player(id, stream);
    objects += p;
    players += p;

    QString s = "Connected players:\n";
    for(auto p: players){
        s += p->getName();
        s += '\n';
    }
    ui->PlayersLabel->setText(s);
}

/*
 * Updates the status text:
 * lists players in order by points, with their status
 */
void MainWindow::updateText(){

    //Set the info label text
    QString s = QString("Game running! t: %1").arg(timeElapsed);
    ui->InfoLabel->setText(s);

    s = "Players:\n";

    //sort players by points, in descending order
    std::sort(players.begin(), players.end(), [](const player* p1, const player* p2) {
        //TODO change to points
        return p1->getId() > p2->getId();
    });

    //add all players' info to the text
    for(auto p: players){
        s += p->getName() + ": ";

        if(p->getIsDead())
            s += "DEAD";

        //TODO change to points
        s += QString("%1 points\n").arg(p->getId());

    }
    ui->PlayersLabel->setText(s);
}

/*
 * Main loop content, called once every FRAME_TIME ms
 */
void MainWindow::executeTurn(){

    if(!running)
        return;

    int dt = time->elapsed();
    time->restart();
    timeElapsed += dt;
    qDebug() << "Doing a turn! dt: " << dt;

    for(auto object: objects)
        object->doStep(dt);

    for(auto object: objects)
        object->move(dt, *tcpmanager);

    updateText();

    tcpmanager->flush();
}

