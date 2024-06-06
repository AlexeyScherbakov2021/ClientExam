#include "settingdlg.h"
#include "ui_settingdlg.h"

SettingDlg::SettingDlg(QString &host, int &port, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDlg), host(&host), port(&port)
{
    ui->setupUi(this);
    ui->leHost->setText(host);
    ui->lePort->setText(QString::number(port));
}

SettingDlg::~SettingDlg()
{
    delete ui;
}

void SettingDlg::on_pushButtonOK_clicked()
{
    bool ok;

    *host = ui->leHost->text();
    *port = ui->lePort->text().toInt(&ok);
    if(ok)
        accept();
}

