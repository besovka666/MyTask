#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HelpInfo.h"
#include "ui_HelpInfo.h"
#include <QShortcut>

#define NOTE_HEIGHT 40


//Контекстное меню, если пользователь нажал на элемент правой кнопкой мыши
void MainWindow::ProvideContextMenu(const QPoint &pos)
{
    QPoint item = ui->listTasks->mapToGlobal(pos);

    QMenu menu;
    menu.addAction("Удалить");

    if (ui->listTasks->count() != 0){

        QAction *rightClick = menu.exec(item);
        if (rightClick && rightClick->text().contains("Удалить")){
            QMessageBox::StandardButton msg = QMessageBox::question(this, "Удалить",
                                                                    "Вы уверены что хотите удалить '" + ui->listTasks->currentItem()->text() + "'?",
                                                                    QMessageBox::Yes | QMessageBox::No);

            if (msg == QMessageBox::Yes){
                maplistTasks.remove(ui->listTasks->currentItem()->text());
                delete ui->listTasks->currentItem();

                if (ui->listTasks->count() == 0){
                    ui->mainText->setText("");
                    maplistTasks.clear();

                    sData->Save(&maplistTasks);
                }
            }
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    popUp = new PopUp();
    connect(ui->actionHelp, SIGNAL(triggered()),this,SLOT(HelpInfo()));

    //ставим пикер времени на текущее время

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    //Строчки для контекстного меню, если пользователь нажал ПКМ на элемент
    ui->listTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ProvideContextMenu(QPoint)));

    KeyF1 = new QShortcut(this);
    KeyF1->setKey(Qt::CTRL + Qt::Key_N);
    connect(KeyF1, SIGNAL(activated()), this, SLOT(slotShortcutCreateTask()));

    KeyDelete = new QShortcut(this);
    KeyDelete->setKey(Qt::CTRL + Qt::Key_D);
    connect(KeyDelete, SIGNAL(activated()), this, SLOT(slotShortcutDEL()));

    KeyClose = new QShortcut(this);
    KeyClose->setKey(Qt::CTRL + Qt::Key_Q);
    connect(KeyClose, SIGNAL(activated()), this, SLOT(slotShortcutClose()));

    KeyBold = new QShortcut(this);
    KeyBold->setKey(Qt::Key_F1);
    connect(KeyBold, SIGNAL(activated()), this, SLOT(slotShortcutBold()));

    KeyItalic = new QShortcut(this);
    KeyItalic->setKey(Qt::Key_F2);
    connect(KeyItalic, SIGNAL(activated()), this, SLOT(slotShortcutItalic()));

    KeyLine = new QShortcut(this);
    KeyLine->setKey(Qt::Key_F3);
    connect(KeyLine, SIGNAL(activated()), this, SLOT(slotShortcutLine()));

    KeyStrike = new QShortcut(this);
    KeyStrike->setKey(Qt::Key_F4);
    connect(KeyStrike, SIGNAL(activated()), this, SLOT(slotShortcutStrike()));

    KeyHelpInfo = new QShortcut(this);
    KeyHelpInfo->setKey(Qt::Key_F5);
    connect(KeyHelpInfo, SIGNAL(activated()), this, SLOT(slotShortcutHelpInfo()));

    KeyF10 = new QShortcut(this);
    KeyF10->setKey(Qt::Key_F10);
    connect(KeyF10, SIGNAL(activated()), this, SLOT(testpopup()));

    sData = new SaveData();
    lData = new LoadData();

    //Загружаем заметки из файла
    if (lData->Load()){
        maplistTasks = lData->getlistTasks();
        int count = maplistTasks.count();

        QString noteName = "";
        int badNote = 0; //badNote - нужен т.к. есть баг, который создает пустую заметку без текста.
                         //Мы же будем вычислять пустые заметки (созданные ошибочно), и удалять
        for (int i = 0; i < count; i++){
            noteName = maplistTasks.keys().at(i);
            if (!noteName.isEmpty()) //Проверяем имя заметки на пустоту. Если все ОК, то добавляем
            {
                ui->listTasks->addItem(noteName);
            }
            else //Иначе записываем в badNote, то что мусорных заметок N
                badNote++;
        }

        //Устанавливаем высоту заметок
        for (int i = 0; i < count - badNote; i++){
            ui->listTasks->item(i)->setSizeHint(QSize(0, NOTE_HEIGHT));

            ui->listTasks->item(i)->setIcon(QIcon(NOTE_ICON));
        }
        //Выделяем заметку
        ui->listTasks->setCurrentRow(0);

        //И автоматом заполняем заметку текстом
        ui->mainText->setText(maplistTasks.value(ui->listTasks->currentItem()->text()).first);
        ui->dateTimeEdit->setDateTime(maplistTasks.value(ui->listTasks->currentItem()->text()).second);

        if (maplistTasks.value(ui->listTasks->currentItem()->text()).second >= QDateTime::currentDateTime()) {
            QSharedPointer<QTimer> timer = QSharedPointer<QTimer>(new QTimer);
            timer->setSingleShot(true);
            timer->start(QDateTime::currentDateTime().msecsTo(
                                   maplistTasks.value(ui->listTasks->currentItem()->text()).second));
            connect(&*timer, &QTimer::timeout, this, [&]() {
                popUp->setPopupText(maplistTasks.value(ui->listTasks->currentItem()->text()).first);
                popUp->show();
            });
            timers_list.push_back(timer);
        }
    }
}

void MainWindow::HelpInfo()
{
    Help *dq = new Help();
    dq->show();
}


MainWindow::~MainWindow()
{
    //Перед выходом сохранимся
    if (ui->listTasks->count() != 0){
        maplistTasks.insert(ui->listTasks->currentItem()->text(),
                            QPair<QString, QDateTime>(ui->mainText->toHtml(), ui->dateTimeEdit->dateTime()));
        sData->Save(&maplistTasks);
    }

    delete ui;
}



void MainWindow::on_btnCreateANote_clicked()
{
    timers_list.clear();
    //Перед созданием новой заметки, сохраняем текущую заметку
    if (ui->listTasks->count() != 0){
        maplistTasks.insert(ui->listTasks->currentItem()->text(),
                            QPair<QString, QDateTime>(ui->mainText->toHtml(), ui->dateTimeEdit->dateTime()));
        sData->Save(&maplistTasks);
    }

    //Вызываем диалоговое окно и получаем имя нашей новой заметки
    crNote = new CreateNote();
    crNote->exec();

    QString noteName = crNote->getName();
    if (noteName.isEmpty())
        return;

    ui->listTasks->addItem(noteName); //Добавляем в QListWidget


    int lastAddedItem = ui->listTasks->count() - 1; //Это будет index последнего добавленного элемента
    ui->listTasks->item(lastAddedItem)->setSizeHint(QSize(0, NOTE_HEIGHT)); //Меняем размер элемента
    ui->listTasks->setCurrentRow(lastAddedItem); //Выделяем элемент
    ui->listTasks->item(lastAddedItem)->setIcon(QIcon(NOTE_ICON));
    ui->mainText->setFocus();

    maplistTasks.insert(noteName,
                        QPair<QString, QDateTime>("", ui->dateTimeEdit->dateTime())); //Добавляем имя заметки в QMap
    ui->mainText->setText("");

    if (maplistTasks.value(ui->listTasks->currentItem()->text()).second >= QDateTime::currentDateTime()) {
        if (ui->listTasks->count() != 0){
            QSharedPointer<QTimer> timer = QSharedPointer<QTimer>(new QTimer);
            timer->setSingleShot(true);
            timer->start(maplistTasks.value(ui->listTasks->currentItem()->text()).second.msecsTo(QDateTime::currentDateTime()));
            connect(&*timer, &QTimer::timeout, this, [&]() {
                qDebug() << "Timeout!" << endl;
                popUp->setPopupText(maplistTasks.value(ui->listTasks->currentItem()->text()).first);
                popUp->show();
            });
            timers_list.push_back(timer);
        }
    }

    lastNote = noteName;

    delete crNote;
}

void MainWindow::on_btnSetBold_clicked()
{
    //Определяем размер шрифта
    QTextCharFormat cur = ui->mainText->currentCharFormat();
    QString text = ui->mainText->textCursor().selectedText();

    //Проверка на выделенный текст
    //50 - не выделенный текст
    if (cur.fontWeight() == 50)
        ui->mainText->insertHtml("<b>" + text + "</b>");
    else
        ui->mainText->insertHtml(text);
}

void MainWindow::on_btnSetItalic_clicked()
{
    QTextCharFormat cur = ui->mainText->currentCharFormat();
    QString text = ui->mainText->textCursor().selectedText();

    //Определяем, курсивом написан текст или нет
    if (!cur.fontItalic())
        ui->mainText->insertHtml("<i>" + text + "</i>"); //<i></i> - теги из HTML
    else
        ui->mainText->insertHtml(text);
}

void MainWindow::on_btnUnderLine_clicked()
{
    QTextCharFormat cur = ui->mainText->currentCharFormat();
    QString text = ui->mainText->textCursor().selectedText();

    //Определяем, подчеркнут текст или нет
    if (!cur.fontUnderline())
        ui->mainText->insertHtml("<u>" + text + "</u>"); //<u></u> - теги из HTML
    else
        ui->mainText->insertHtml(text);
}

void MainWindow::on_btnStrike_clicked()
{
    QTextCharFormat cur = ui->mainText->currentCharFormat();
    QString text = ui->mainText->textCursor().selectedText();

    //Определяем, зачеркнутый текст или нет
    if (!cur.fontStrikeOut())
        ui->mainText->insertHtml("<s>" + text + "</s>"); //<s></s> - теги из HTML
    else
        ui->mainText->insertHtml(text);
}

void MainWindow::on_listTasks_itemClicked(QListWidgetItem *item)
{
    //Перед переключением на другую заметку, сохраним текущую
    if (ui->listTasks->count() != 0){
        maplistTasks.insert(lastNote, QPair<QString, QDateTime>(ui->mainText->toHtml(), ui->dateTimeEdit->dateTime()));
        sData->Save(&maplistTasks);
    }

    ui->mainText->setFocus();
    ui->mainText->setText(maplistTasks.value(item->text()).first); //Если это старая заметка, то загружаем текст из QMap
    ui->dateTimeEdit->setDateTime(maplistTasks.value(item->text()).second);

    lastNote = item->text();
}

void MainWindow::testpopup()
{
    popUp->setPopupText(ui->mainText->toPlainText());
    popUp->show();
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if (dateTime < QDateTime::currentDateTime()) {
        popUp->setPopupText("Напоминайте себе о предстоящих делах - прошлое само о себе напомнит :)");
        popUp->show();
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
    else {
        if (ui->listTasks->count() != 0){
            QSharedPointer<QTimer> timer = QSharedPointer<QTimer>(new QTimer);
            timer->setSingleShot(true);
            timer->start(std::abs(maplistTasks.value(ui->listTasks->currentItem()->text()).second.msecsTo(QDateTime::currentDateTime())));
            qDebug() << "Timer has been set up" << dateTime << endl;
            if (dateTime > QDateTime::currentDateTime()) {
                popUp->setPopupText("Таймер установлен");
                popUp->show();
            }
            QString textToDisplay = maplistTasks.value(ui->listTasks->currentItem()->text()).first;
            connect(&*timer, &QTimer::timeout, this, [=]() {
                qDebug() << "Timeout!" << endl;
                popUp->setPopupText(textToDisplay);
                popUp->show();
            });
            timers_list.push_back(timer);
        }
    }
}

void MainWindow::slotShortcutCreateTask()
{
    MainWindow::on_btnCreateANote_clicked();
}

void MainWindow::slotShortcutDEL()
{
    if (ui->listTasks->count() != 0){
    maplistTasks.remove(ui->listTasks->currentItem()->text());
    delete ui->listTasks->currentItem();

    if (ui->listTasks->count() == 0) {
        ui->mainText->setText("");
        maplistTasks.clear();

        sData->Save(&maplistTasks);  }
    }
    else {
          popUp->setPopupText("Куда ещё удалять итак всё пусто...");
          popUp->show();
         }
}

void MainWindow::slotShortcutBold()
{
  MainWindow::on_btnSetBold_clicked();
}

void MainWindow::slotShortcutItalic()
{
  MainWindow::on_btnSetItalic_clicked();
}

void MainWindow::slotShortcutLine()
{
  MainWindow::on_btnUnderLine_clicked();
}

void MainWindow::slotShortcutStrike()
{
  MainWindow::on_btnStrike_clicked();
}

void MainWindow::slotShortcutClose()
{
    close();
}

void MainWindow::slotShortcutHelpInfo()
{
    MainWindow::HelpInfo();
}


