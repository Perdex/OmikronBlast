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
    //TODO needs to be changed to map->regenerate etc to not mess up pointers
    map = new Map(ui->mapView);
    for(player *p: players){
        p->resetPosition();
    }
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
        QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);
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
void MainWindow::addPlayer(QDataStream *stream, qint16 id){
    player* p = new player(id, stream, map, this);
    objects[id] = p;
    players[id] = p;

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
    p->deleteLater();
}

qint16 MainWindow::getNextId(){
    nextId++;
    return nextId;
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

    if(!running)
        return;

    int dt = time->elapsed();
    dt = qMin(dt, 50);
    time->restart();
    timeElapsed += dt;
    //qDebug() << "Doing a turn! dt: " << dt;

    QVector<stuff*> toBeRemoved;

    for(auto object: objects)
        if(object->doStep(dt))
            toBeRemoved += object;

    for(auto object: objects)
        object->move(dt, *tcpmanager);

    for(auto object: toBeRemoved)
        remove(object);

    updateText();

    tcpmanager->flush();
    QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);
}


void MainWindow::remove(stuff *s) {
    objects.remove(s->getId());
    if(s->getType() == Stuff::PLAYER)
        players.remove(s->getId());
    delete s;
    // TODO Fix memory leak
}
