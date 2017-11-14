#include "frame.h"
#include "ui_frame.h"
#include <QtDebug>

#include <QObject>

// This is really WIP and should only be used as a sandbox
// Like this entire class currently
void Frame::switchView() {
    ui->views->setCurrentIndex(1);
    QGraphicsScene *scene = new QGraphicsScene(0, 0, 8000, 8000, ui->canvas);
    ui->canvas->setScene(scene);

    ui->canvas->centerOn(5000, 5000); // Use this to move

    scene->addLine(0, 0, 100, 100);
    scene->addLine(0, 0, 5100, 5100);

    scene->invalidate();
}

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    QObject::connect(ui->commandLinkButton, &QCommandLinkButton::pressed, this, &Frame::switchView);
}

void Frame::onConnected() {

}

Frame::~Frame()
{
    delete ui;
}
