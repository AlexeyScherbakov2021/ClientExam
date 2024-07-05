#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "exsmenquest.h"
#include "helpwindow.h"
#include "timerwindow.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool isExamiing = false;
    bool isConnected = false;
    QTcpSocket *socket;
    QLabel *lbStatus;
    QString host;
    int port;
    QLabel *lbItem;
    QPointer<HelpWindow> helpWin;
    TimerWindow *win = nullptr;


    QList<ExamQuest> listExam;
    int timeDuration;
    int leftTimes;
    QTimer timer;
    QTimer timerConnect;
    const QString startButton = "Запуск";
    const QString finishButton = "Завершить";
    QString urlSchema;

    void setTimerCount();
    void ReadSetting();
    void SaveSetting(const QString &hst, int prt );

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();
    void slotTimeOut();
    void slotTimeConnect();

    void on_cbBilet_currentIndexChanged(int index);
    void on_pushButtonStart_clicked();
    void on_pushButtonNew_clicked();
    void on_pushButtonSet_clicked();
    void on_pushButtonToShema_clicked();
    void on_pushButtonHelp_clicked();

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

signals:
    void sigTimerText(QString sTime);
};

#endif // MAINWINDOW_H
