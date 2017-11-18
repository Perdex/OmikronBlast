#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TCPManager.h"

#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpmanager(new TCPManager()),
    gameLoopTimer(nullptr)
{
    ui->setupUi(this);

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
}

void MainWindow::startGame(){

    qDebug() << "Starting game";

    gameLoopTimer->start(FRAME_TIME);

}

void MainWindow::executeTurn(){

    qDebug() << "Doing a turn!";
    tcpmanager->readData();

    //jotain tällasta tänne:
    /*
    for(auto object: objects){
        object.doturn(tcpmanager);
    }
    for(auto object: objects){
        object.move(tcpmanager);
    }
    */
    tcpmanager->sendData();
}

