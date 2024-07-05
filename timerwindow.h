#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

// #include "mainwindow.h"

#include <QWidget>

namespace Ui {
class TimerWindow;
}

class TimerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TimerWindow(const QString &sTime, QWidget *parent = nullptr);
    ~TimerWindow();

private:
    Ui::TimerWindow *ui;
    QPoint oldPos;
    QWidget *mainWin;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private slots:
    void slotRefreshTime(QString sTime);

};

#endif // TIMERWINDOW_H
