#include "frame.h"
#include "ui_frame.h"
#include <QtDebug>

#include <QObject>

void Frame::switchView() {

}

void Frame::dbg(const QMap<int, bool> &keys) {
    qDebug() << keys;
}

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);

    ui->views->setCurrentIndex(0);

    QObject::connect(ui->quitButton, &QPushButton::clicked, this, &Frame::onQuit);
    QObject::connect(ui->commandLinkButton, &QCommandLinkButton::pressed, this, &Frame::onConnectClicked);
    QObject::connect(ui->canvas, &Canvas::keysChanged, this, &Frame::dbg);
}

void Frame::onConnectClicked() {
    ui->status->setText("Trying to connect...");
    emit requestConnection(ui->s_addr->text(), ui->s_port->text().toInt());
}

void Frame::onConnected() {
    ui->status->setText("Connected.");

    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));
    ui->views->setCurrentIndex(1);
}

void Frame::onQuit() {
    this->close();
}

Frame::~Frame()
{
    delete ui;
}
