#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "exsmenquest.h"

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

private slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();
    void slotTimeOut();
    void slotTimeConnect();

    void on_cbBilet_currentIndexChanged(int index);
    void on_pushButtonStart_clicked();
    void on_pushButtonNew_clicked();
    void on_pushButtonSet_clicked();
    void on_pushButtonToShema_clicked();
};

#endif // MAINWINDOW_H
