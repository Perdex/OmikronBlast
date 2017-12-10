#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmanager.h"
#include "player.h"
#include "stuff.h"
#include "map.h"
#include "message.h"

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
    started(false)
{
    ui->setupUi(this);

    tcpmanager = new TCPManager(this);

    QString s = "Server active at:\n";
    s += tcpmanager->getAddress();
    s += "\nport: ";
    s += tcpmanager->getPort();
    ui->InfoLabel->setText(s);

    map = new Map(ui->mapView);

    //initialize and connect the main loop timer: will not run yet
    //gameLoopTimer = new QTimer(this);
    //QObject::connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(executeTurn()));

    //connect the start button to start game
    QObject::connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    QObject::connect(ui->regenMapButton, &QPushButton::clicked, this, &MainWindow::generateMap);
}

void MainWindow::generateMap() {
    map = new Map(ui->mapView);
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
        if(!started){
            qDebug() << "Starting game";

            started = true;
            tcpmanager->gameStarted();

            StatusMessage *start_msg = new StatusMessage(GameStatus::START);
            qDebug() << start_msg;
            qDebug() << (qint8)start_msg->status();
            *tcpmanager << start_msg;
            //delete start_msg;

            //Ei saa poistaa kommenteista ennen kuin client osaa
            //vastaanottaa kartan
            map->send(tcpmanager);

            time = new QTime();
            time->start();
        }else{
            qDebug() << "Continuing game";

            time->restart();
        }

        ui->startButton->setText("Pause game");
        QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);
    }else{
        //pause
        ui->startButton->setText("Continue game");
        running = false;
    }
}

void MainWindow::endGame(){
    qDebug() << "Ending the game!";
    running = false;
    this->deleteLater();
    //gameLoopTimer->stop();
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
    QString s = QString("Game running! t: %1 s").arg(timeElapsed / 1000);
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
    //qDebug() << "Doing a turn! dt: " << dt;

    for(auto object: objects)
        object->doStep(dt);

    for(auto object: objects)
        object->move(dt, *tcpmanager);

    updateText();

    tcpmanager->flush();
    QTimer::singleShot(FRAME_TIME, this, &MainWindow::executeTurn);
}

