#include "mainwindow.h"
#include "settingdlg.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ReadSetting();

    QString lineConnect = QString("Соединение с сервером (%1:%2):").arg(host).arg(port);
    lbItem = new QLabel(lineConnect, this);
    lbItem->setContentsMargins(6,0,0,0);
    ui->statusbar->addWidget(lbItem);

    lbStatus = new QLabel(this);
    ui->statusbar->addWidget(lbStatus);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(slotDisconnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));

    slotTimeConnect();

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

void MainWindow::ReadSetting()
{
    QString s = QApplication::applicationName() + ".ini";
    QSettings set(s, QSettings::IniFormat);
    set.beginGroup("Server");
    host = set.value("Host", "127.0.0.1").toString();
    port = set.value("Port", 24242).toInt();
    urlSchema = set.value("Schema").toString();
    set.endGroup();

}



//---------------------------------------------------------------------------------------------------------------
// Запись параметров в файл INI
//---------------------------------------------------------------------------------------------------------------
void MainWindow::SaveSetting(const QString &hst, int prt)
{
    QString s = QApplication::applicationName() + ".ini";
    QSettings set(s, QSettings::IniFormat);
    // QSettings set(QSettings::NativeFormat, QSettings::UserScope, "", QApplication::applicationName());
    set.beginGroup("Server");
    set.setValue("Host", hst);
    set.setValue("Port", prt);
    set.setValue("Schema", urlSchema);
    set.endGroup();

}

//---------------------------------------------------------------------------------------------------------------
// событие закрытия окна
//---------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isExamiing)
        event->ignore();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1)
        on_pushButtonHelp_clicked();
}


//---------------------------------------------------------------------------------------------------------------
// событие присоединения клиента
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotConnected()
{
    timerConnect.stop();
    lbStatus->setText("установлено");
    isConnected = true;
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStart->setText(startButton);
    ui->pushButtonStart->setChecked(false);

}

//---------------------------------------------------------------------------------------------------------------
// событие закрытия соединения
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotDisconnected()
{
    isConnected = false;
    lbStatus->setText("нет соединения");
    ui->pushButtonStart->setEnabled(false);
    timerConnect.setInterval(3000);
    connect(&timerConnect, SIGNAL(timeout()), SLOT(slotTimeConnect()));
    timer.stop();
    isExamiing = false;
    timerConnect.start();
}

//---------------------------------------------------------------------------------------------------------------
// Ошибка сетевого соединения
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotError(QAbstractSocket::SocketError)
{
    if(socket->state() != QAbstractSocket::ConnectingState && !timerConnect.isActive())
    {
        slotDisconnected();
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
    leftTimes = timeDuration;
    setTimerCount();

}

//---------------------------------------------------------------------------------------------------------------
// Отсчет таймера закончен
//---------------------------------------------------------------------------------------------------------------
void MainWindow::slotTimeOut()
{
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
    socket->connectToHost(host, port);
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

    for(int i = 0; i < listExam[index].listAnswer.size(); ++i)
        ui->cbOtvet->insertItem(i, listExam[index].listAnswer[i].e_value);
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
        ui->pushButtonStart->setText(startButton);
        typePacket = 3;
        out << typePacket;
        isExamiing = false;
    }
    else
    {
        // начало экзамена
        isExamiing = true;
        ui->pushButtonStart->setText(finishButton);
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
    ui->pushButtonStart->setText(startButton);
    ui->pushButtonStart->setEnabled(isConnected);
    ui->leLastName->clear();
    ui->leName->clear();
    ui->leMidName->clear();
    ui->leDO->clear();
    leftTimes = timeDuration;
    setTimerCount();
}


void MainWindow::on_pushButtonSet_clicked()
{
    QString hostSet = host;
    int portSet = port;

    SettingDlg dlg(hostSet, portSet, urlSchema);
    if(dlg.exec() == 1)
    {
        host = hostSet;
        port = portSet;
        SaveSetting(hostSet, portSet);
        QString lineConnect = QString("Соединение с сервером (%1:%2):").arg(host).arg(port);
        lbItem->setText(lineConnect);
        socket->close();

    }
}


void MainWindow::on_pushButtonToShema_clicked()
{
    if(!urlSchema.isEmpty())
        QDesktopServices::openUrl(QUrl(urlSchema));
}


void MainWindow::on_pushButtonHelp_clicked()
{
    if(helpWin.isNull() || helpWin.get() == nullptr)
    {
        helpWin = new HelpWindow("index.html");
        helpWin->setAttribute(Qt::WA_DeleteOnClose);
        helpWin->show();
    }
    else
        helpWin->activateWindow();

}

