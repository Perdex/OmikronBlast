#include "frame.h"
#include "ui_frame.h"

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
}

Frame::~Frame()
{
    delete ui;
}
