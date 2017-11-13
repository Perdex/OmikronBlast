#include "frame.h"
#include "ui_frame.h"

Frame::Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
}

Frame::~Frame()
{
    delete ui;
}
