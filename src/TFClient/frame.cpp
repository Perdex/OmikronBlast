#include "frame.h"
#include "ui_frame.h"
#include <QtDebug>

#include <QObject>

void Frame::switchView() {
    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));
    ui->views->setCurrentIndex(1);
}

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);

    ui->views->setCurrentIndex(0);

    QObject::connect(ui->quitButton, &QPushButton::clicked, this, &Frame::onQuit);
    QObject::connect(ui->commandLinkButton, &QCommandLinkButton::pressed, this, &Frame::switchView);
}

void Frame::onConnected() {

}

void Frame::onQuit() {
    this->close();
}

Frame::~Frame()
{
    delete ui;
}
