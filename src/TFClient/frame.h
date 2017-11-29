#ifndef FRAME_H
#define FRAME_H

#include <QMainWindow>
#include "canvas.h"

namespace Ui {
class Frame;
}

class Frame : public QMainWindow
{
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = 0);
    ~Frame();
    Canvas& getCanvas() const;
public slots:
    void onConnected();
    void switchView();
    void setStatus(QString);
private slots:
    void onQuit();
    void dbg(const QMap<int, bool>&);
    void onConnectClicked();
signals:
    void requestConnection(const QString, const int);

private:
    Ui::Frame *ui;
};

#endif // FRAME_H
