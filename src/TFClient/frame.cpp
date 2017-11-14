#include "frame.h"
#include "ui_frame.h"
#include <QtDebug>

#include <QObject>

void Frame::switchView() {
    ui->views->setCurrentIndex(1);
    qDebug() << "Curr: " << ui->views->currentIndex() << "\n";
}

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    QObject::connect(ui->commandLinkButton, &QCommandLinkButton::pressed, this, &Frame::switchView);

    qDebug() << "Curr: " << ui->views->currentIndex() << "\n";
}

Frame::~Frame()
{
    delete ui;
}
