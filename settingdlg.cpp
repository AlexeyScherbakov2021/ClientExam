#include "settingdlg.h"
#include "ui_settingdlg.h"

SettingDlg::SettingDlg(QString &host, int &port, QString &url, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDlg), host(&host), port(&port), urlSchema(&url)
{
    ui->setupUi(this);
    ui->leHost->setText(host);
    ui->lePort->setText(QString::number(port));
    ui->leSchema->setText(*urlSchema);
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
    *urlSchema = ui->leSchema->text();
    if(ok)
        accept();
}

