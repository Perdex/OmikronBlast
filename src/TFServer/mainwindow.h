#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define FRAME_TIME 20

namespace Ui {
    class MainWindow;
}
class TCPManager;
class QTimer;
class QTcpSocket;
class stuff;
class player;
class Map;
class QTime;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addPlayer(qint16 id, QDataStream *stream);
    void endGame();
public slots:
    void startGame();
    void executeTurn();
    void generateMap();

private:
    Ui::MainWindow *ui;
    TCPManager *tcpmanager;
    //QTimer *gameLoopTimer;
    QTime *time;
    int timeElapsed;
    Map *map;
    bool running;
    bool started;

    //objects includes also players
    QVector<stuff*> objects;
    //players, sorted by points
    QVector<player*> players;

    void updateText();
};

#endif // MAINWINDOW_H
