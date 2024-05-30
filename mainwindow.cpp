#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    host = "192.168.102.167";

    QLabel *lbItem = new QLabel("Соединение с сервером:", this);
    lbItem->setContentsMargins(6,0,0,0);
    ui->statusbar->addWidget(lbItem);

    lbStatus = new QLabel(this);
    ui->statusbar->addWidget(lbStatus);

    socket = new QTcpSocket(this);


    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));

    slotTimeConnect();
    // socket->connectToHost(host, 24242);


    connect(&timer, SIGNAL(timeout()), SLOT(slotTimeOut()));
    timer.setInterval(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTimerCount()
{
    QTime time(0,0,0);
    time = time.addSecs(leftTimes);
    ui->lbTime->setText(time.toString("mm:ss"));
}

//---------------------------------------------------------------------------------------------------------------
// событие закрытия окна
//---------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isExamiing)
        event->ignore();
}


//---------------------------------------------------------------------------------------------------------------
// событие присоединения коиента
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotConnected()
{
    timerConnect.stop();
    // disconnect(&timerConnect, SIGNAL(timeout()), SLOT(slotTimeConnect()));
    lbStatus->setText("установлено " + host);
    ui->pushButtonStart->setEnabled(true);

    // lbStatus->setStyleSheet("");
}

//---------------------------------------------------------------------------------------------------------------
// Ошибка сетевого соединения
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotError(QAbstractSocket::SocketError)
{
    if(socket->state() != QAbstractSocket::ConnectingState && !timerConnect.isActive())
    {
        lbStatus->setText("нет соединения");
        ui->pushButtonStart->setEnabled(false);
        timerConnect.setInterval(3000);
        connect(&timerConnect, SIGNAL(timeout()), SLOT(slotTimeConnect()));
        timerConnect.start();
    }
}

//---------------------------------------------------------------------------------------------------------------
// событие получения данных от клиента
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotReadyRead()
{
    int typePacket;
    // int duration;
    int countQuest;
    int countAns;
    QStringList sl;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);

    in >> typePacket >> timeDuration >> countQuest;

    listExam.clear();
    ui->cbBilet->clear();
    while(countQuest > 0)
    {
        ExamQuest exam;
        in >> exam.question;
        in >> countAns;
        while(countAns-- > 0)
        {
            ExamAnswer ans;
            in >> ans.e_value;
            exam.listAnswer.append(ans);
        }
        listExam.append(exam);
        ui->cbBilet->insertItem(0, QString("%1").arg(countQuest));
        countQuest--;
    }

    ui->cbBilet->setCurrentIndex(0);

timeDuration = 10;  //////////////////////////////////////////////////////
    leftTimes = timeDuration;
    setTimerCount();

    // QTime time(0,0,0);
    // timeDuration = time.addSecs(duration);
    // ui->lbTime->setText(timeDuration.toString("mm:ss"));

}

//---------------------------------------------------------------------------------------------------------------
// Отсчет таймера закончен
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotTimeOut()
{
    // timer.stop();
    --leftTimes;
    setTimerCount();
    if(leftTimes == 0)
    {
        timer.stop();
        ui->pushButtonStart->setChecked(false);
        on_pushButtonStart_clicked();
    }
}

//---------------------------------------------------------------------------------------------------------------
// таймер сетевого соединения
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotTimeConnect()
{
    socket->connectToHost(host, 24242);
}


//---------------------------------------------------------------------------------------------------------------
// изменение выбранного вопроса
//---------------------------------------------------------------------------------------------------------------
void MainWindow::on_cbBilet_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    ui->lbContentBilet->setText(listExam[index].question);
    ui->cbOtvet->clear();
    for(auto &it : listExam[index].listAnswer)
    {
        ui->cbOtvet->insertItem(-1, it.e_value);
    }
}



//---------------------------------------------------------------------------------------------------------------
// кнопка Старт
//---------------------------------------------------------------------------------------------------------------
void MainWindow::on_pushButtonStart_clicked()
{
    int typePacket;
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    if(!ui->pushButtonStart->isChecked())
    {
        // Завершение экзамена
        timer.stop();
        ui->pushButtonStart->setEnabled(false);
        typePacket = 3;
        out << typePacket;
        isExamiing = false;
    }
    else
    {
        // начало экзамена
        isExamiing = true;
        ui->pushButtonStart->setText("Завершить");
        typePacket = 2;
        int indexAnswer = ui->cbOtvet->currentIndex();
        int indexQuest = ui->cbBilet->currentIndex();

        QString lastName = ui->leLastName->text();
        QString name = ui->leName->text();
        QString midName = ui->leMidName->text();
        QString DO = ui->leDO->text();

        out << typePacket << lastName << name << midName << DO << indexQuest << indexAnswer;
        leftTimes = timeDuration;
        timer.start();
    }

    socket->write(arrBlock);
}


//---------------------------------------------------------------------------------------------------------------
// конпка Новый
//---------------------------------------------------------------------------------------------------------------
void MainWindow::on_pushButtonNew_clicked()
{
    ui->pushButtonStart->setText("Запуск");
    ui->pushButtonStart->setEnabled(true);
    ui->leLastName->clear();
    ui->leName->clear();
    ui->leMidName->clear();
    ui->leDO->clear();
    leftTimes = timeDuration;
    setTimerCount();
}

