#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDebug>
#include <QMap>
#include <QMessageBox>
#include <QShortcut>

#include "popup.h"
#include "createnote.h"
#include "savedata.h"
#include "loaddata.h"

#define NOTE_ICON ":/images/icon_note.png"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CreateNote *crNote;
    SaveData *sData;
    LoadData *lData;

    QMap<QString, QPair<QString, QDateTime>> maplistTasks; //<Имя заметки, текст>

    QString lastNote = "";




public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void ProvideContextMenu(const QPoint &pos);

    void on_btnCreateANote_clicked();

    void on_btnSetBold_clicked();

    void on_btnSetItalic_clicked();

    void on_btnUnderLine_clicked();

    void on_btnStrike_clicked();

    void on_listTasks_itemClicked(QListWidgetItem *item);

    void testpopup();

    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void slotShortcutCreateTask();

    void slotShortcutDEL();

    void slotShortcutBold();

    void slotShortcutItalic();

    void slotShortcutLine();

    void slotShortcutStrike();

    void slotShortcutClose();

    void HelpInfo();

    void slotShortcutHelpInfo();


private:
    Ui::MainWindow *ui;
    PopUp *popUp; // Объявляем объект всплывающего сообщения
    QList<QSharedPointer<QTimer>> timers_list;
    QShortcut *KeyF1;
    QShortcut *KeyDelete;
    QShortcut *KeyBold;
    QShortcut *KeyItalic;
    QShortcut *KeyLine;
    QShortcut *KeyStrike;
    QShortcut *KeyClose;
    QShortcut *KeyHelpInfo;
    QShortcut *KeyF10;

};

#endif // MAINWINDOW_H
