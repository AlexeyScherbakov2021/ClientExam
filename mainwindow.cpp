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
    socket->connectToHost(host, 24242);

    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isExamiing)
        event->ignore();
}


void MainWindow::slotConnected()
{
    lbStatus->setText("установлено " + host);
}

void MainWindow::slotError(QAbstractSocket::SocketError)
{

}

void MainWindow::slotReadyRead()
{
    int typePacket;
    int duration;
    int countQuest;
    int countAns;
    QStringList sl;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);

    in >> typePacket >> duration >> countQuest;

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

}

void MainWindow::on_cbBilet_currentIndexChanged(int index)
{
    ui->lbContentBilet->setText(listExam[index].question);
    ui->cbOtvet->clear();
    for(auto &it : listExam[index].listAnswer)
    {
        ui->cbOtvet->insertItem(-1, it.e_value);
    }

}


void MainWindow::on_pushButtonStart_clicked()
{
    int typePacket;
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);


    if(!ui->pushButtonStart->isChecked())
    {
        ui->pushButtonStart->setEnabled(false);
        typePacket = 2;
        out << typePacket;
    }
    else
    {
        ui->pushButtonStart->setText("Завершить");
        typePacket = 3;
        int indexAnswer = ui->cbOtvet->currentIndex();
        int indexQuest = ui->cbBilet->currentIndex();

        QString lastName = ui->leLastName->text();
        QString name = ui->leName->text();
        QString midName = ui->leMidName->text();
        QString DO = ui->leDO->text();

        out << typePacket << lastName << name << midName << indexQuest << indexAnswer;
    }

    socket->write(arrBlock);

}


void MainWindow::on_pushButtonNew_clicked()
{
    ui->pushButtonStart->setText("Запуск");
    ui->pushButtonStart->setEnabled(true);
    ui->leLastName->clear();
    ui->leName->clear();
    ui->leMidName->clear();
    ui->leDO->clear();
}

