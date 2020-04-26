#ifndef CREATENOTE_H
#define CREATENOTE_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class CreateNote;
}

class CreateNote : public QDialog
{
    Q_OBJECT

    QString name;

public:
    explicit CreateNote(QWidget *parent = nullptr);
    ~CreateNote();

    QString getName() const;

    void ShowMessage(QString str);

private slots:
    void on_btnCreate_clicked();

private:
    Ui::CreateNote *ui;
};

#endif // CREATENOTE_H
