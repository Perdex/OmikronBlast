#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

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
class projectile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addPlayer(QDataStream *stream, qint16 id);
    void addProjectile(projectile *p);
    void remove(projectile *p);
    void remove(player *p);
    void endGame();
    qint16 getNextId();
    QVector<player*> &getPlayers() { return players; }
    void remove(stuff*);
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
    QMap<qint16, stuff*> objects;
    //players, sorted by points
    QMap<qint16, player*> players;

    qint16 nextId;

    void updateText();
};

#endif // MAINWINDOW_H
