#ifndef FRAME_H
#define FRAME_H

#include <QMainWindow>

namespace Ui {
class Frame;
}

class Frame : public QMainWindow
{
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = 0);
    ~Frame();
    void switchView();
public slots:
    void onConnected();
private slots:
    void onQuit();

private:
    Ui::Frame *ui;
};

#endif // FRAME_H
