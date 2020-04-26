#include "createnote.h"
#include "ui_createnote.h"
#include <QMessageBox>
#define MAX_NOTE_LEN 24

QString CreateNote::getName() const
{
    return name;

}

void CreateNote::ShowMessage(QString str)
{
    QMessageBox msg;
    msg.setText(str);
    msg.exec();
}

CreateNote::CreateNote(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::CreateNote)
{
    ui->setupUi(this);
}

CreateNote::~CreateNote()
{
    delete ui;
}

void CreateNote::on_btnCreate_clicked()
{
    //Идет проверка на введенные значения
    if (ui->lineEdit->text().isEmpty())
        QMessageBox::warning(this,"Ошибка", "Поле пустое. Пожалуйста введите имя.");
    else if (ui->lineEdit->text().size() > MAX_NOTE_LEN)
        ShowMessage("Ошибка. Превышен интервал символов. максимальное кол-во символов " + QString::number(MAX_NOTE_LEN) + ".");
    else {
        name = ui->lineEdit->text();

        close();
    }
}
