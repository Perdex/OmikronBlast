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
public slots:
    void onConnected();
    void switchView();
private slots:
    void onQuit();
    void dbg(const QMap<int, bool>&);

private:
    Ui::Frame *ui;
};

#endif // FRAME_H
