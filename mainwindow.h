#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "exsmenquest.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QTcpSocket>

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
    QTcpSocket *socket;
    QLabel *lbStatus;
    QString host;

    QList<ExamQuest> listExam;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();

    void on_cbBilet_currentIndexChanged(int index);
    void on_pushButtonStart_clicked();
    void on_pushButtonNew_clicked();
};

#endif // MAINWINDOW_H