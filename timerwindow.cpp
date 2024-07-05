#include "mainwindow.h"
#include "timerwindow.h"
#include "ui_timerwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

TimerWindow::TimerWindow(const QString &sTime, QWidget *parent)
    : QWidget(nullptr)
    , ui(new Ui::TimerWindow), mainWin(parent)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(3.0);
    shadowEffect->setColor(QColor(0, 0, 0, 190));
    shadowEffect->setOffset(2.0);
    ui->labelTime->setGraphicsEffect(shadowEffect);
    ui->labelTime->setText(sTime);

    MainWindow *win = (MainWindow*) parent;
    connect(win, SIGNAL(sigTimerText(QString)), this, SLOT(slotRefreshTime(QString)));

    QScreen *screen = QApplication::primaryScreen();
    int widthScreen = screen->geometry().width();

    move(widthScreen - this->width(), 40);
}

TimerWindow::~TimerWindow()
{
    delete ui;
}


void TimerWindow::mousePressEvent(QMouseEvent *event)
{
    oldPos = event->pos();
}

void TimerWindow::mouseMoveEvent(QMouseEvent *event)
{
    const QPointF delta = event->globalPosition() - oldPos;
    move(delta.toPoint());
}


void TimerWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    mainWin->showNormal();
    mainWin->activateWindow();
}

void TimerWindow::slotRefreshTime(QString sTime)
{
    ui->labelTime->setText(sTime);
}


