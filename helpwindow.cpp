#include "helpwindow.h"
#include "mainwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QString target, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HelpWindow)
{
    ui->setupUi(this);

    ui->textBrowser->setSearchPaths(QStringList() << "qrc:/help/Resources/help");
    ui->textBrowser->setSource(QUrl(target));

}

HelpWindow::~HelpWindow()
{
    delete ui;
}
