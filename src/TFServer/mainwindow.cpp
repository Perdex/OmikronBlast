#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmanager.h"
#include "player.h"
#include "stuff.h"

#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpmanager(nullptr),
    gameLoopTimer(nullptr)
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
    for(stuff* s: objects){
        delete s;
    }
    objects.clear();
}

void MainWindow::startGame(){
    qDebug() << "Starting game";
    gameLoopTimer->start(FRAME_TIME);
}

/*
 * Updates the window text
 * Used for showing connected clients
 */
void MainWindow::setPlayersText(QString text){
    ui->PlayersLabel->setText(text);
}

/*
 * Adds a player to the game once connected
 * sock: the tcp socket for receiving data
 */
void MainWindow::addPlayer(QDataStream *stream){
    objects += new player(stream);
}

/*
 * Main loop content, called once every FRAME_TIME ms
 */
void MainWindow::executeTurn(){

    qDebug() << "Doing a turn!";

    //jotain tällasta tänne:
/*
    for(auto object: objects){
        object->doturn(tcpmanager);
    }
*/
    for(auto object: objects){
        object->move(tcpmanager);
    }

    tcpmanager->flush();
}

