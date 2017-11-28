#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define FRAME_TIME 200

namespace Ui {
    class MainWindow;
}
class TCPManager;
class QTimer;
class QTcpSocket;
class stuff;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setPlayersText(QString text);
    void addPlayer(QDataStream *stream);
public slots:
    void startGame();
    void executeTurn();

private:
    Ui::MainWindow *ui;
    TCPManager *tcpmanager;
    QTimer *gameLoopTimer;
    //joku tällanen tänne
    QVector<stuff*> objects;
};

#endif // MAINWINDOW_H
