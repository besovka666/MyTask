#include "HelpInfo.h"
#include "ui_helpinfo.h"

Help::Help(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setWindowTitle("HelpInfo");
}

Help::~Help()
{
    delete ui;
}
