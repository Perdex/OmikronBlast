#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define FRAME_TIME 200

namespace Ui {
    class MainWindow;
}
class TCPManager;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startGame();
    void executeTurn();

private:
    Ui::MainWindow *ui;
    TCPManager *tcpmanager;
    QTimer *gameLoopTimer;
    //joku tällanen tänne
    //QVector<GameObject*> objects;
};

#endif // MAINWINDOW_H
