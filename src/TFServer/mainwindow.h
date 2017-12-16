#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#define FRAME_TIME 500

namespace Ui {
    class MainWindow;
}
class UDPManager;
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
    void addPlayer(qint16 id, QString name);
    void addProjectile(projectile *p);
    void remove(projectile *p);
    void remove(player *p);
    void endGame();
    bool isRunning();
    qint16 getNextId();
    QMap<qint16, player*> &getPlayers() { return players; }
    void processUpdate(QDataStream &in);
public slots:
    void startGame();
    void executeTurn();
    void generateMap();
private slots:
    void newRound();
    void startRunning();

private:
    Ui::MainWindow *ui;
    UDPManager *tcpmanager;
    //QTimer *gameLoopTimer;
    QTime *time;
    int timeElapsed;
    Map *map;
    void remove(stuff*);
    bool running;
    bool started;
    int nextFrameTime;
    int roundCount;

    //objects includes also players
    QMap<qint16, stuff*> objects;
    //players, sorted by points
    QMap<qint16, player*> players;
    QVector<player*> deadplayers;

    qint16 nextId;

    void updateText();
};

#endif // MAINWINDOW_H
