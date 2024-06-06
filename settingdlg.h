#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>

namespace Ui {
class SettingDlg;
}

class SettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDlg(QString &host, int &port, QWidget *parent = nullptr);
    ~SettingDlg();

private slots:
    void on_pushButtonOK_clicked();

private:
    Ui::SettingDlg *ui;
    QString *host;
    int *port;
};

#endif // SETTINGDLG_H
