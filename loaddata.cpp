#include "loaddata.h"
#include <QDateTime>
#include <QPair>

#define FOLDER "/HoffmannTasks"
#define NOTES_FILE "/Tasks_data.svk"

QMap<QString, QPair<QString, QDateTime>> LoadData::getlistTasks() const
{
    return listTasks;
}

LoadData::LoadData()
{
    //Узнаем имя пользователя
    userName = QDir::homePath();

}

bool LoadData::Load()
{

    //Ищем файл с сохраненными заметками
    QFile fileNotes(userName + FOLDER + NOTES_FILE);

    //Проверяем, существует файл или нет
    if (!fileNotes.open(QIODevice::ReadOnly)){
        qDebug() << "File '" << fileNotes.fileName() << "' is not found!";
        fileNotes.close();
        return false;
    }

    //Если файл существует, то открываем его в бинарном режиме
    QDataStream binFile(&fileNotes);

    //Переменная для хранения кол-ва заметок
    int count = 0;

    //Считываем кол-во заметок из файла.
    binFile >> count;

    //Если, все таки, файл существует, то это не повод сразу загружать из него заметки.
    //Для начала нужно проверить, сколько заметок в файле. Если 0 то возвращем false
    if (count == 0){
        fileNotes.close();
        return false;
    }

    QString tNote = ""; //Временная переменная для хранения имени заметки
    QString tText = ""; //Временная переменная для хранения текста из заметки
    QString tDate = "";

    QByteArray arrNote;
    QByteArray arrText;
    QByteArray arrDate;

    //Идем по циклу и считываем из файла заметки и текст, а после записываем во временный контейнер
    for (int i = 0; i < count; i++){
        binFile >> arrNote;
        binFile >> arrText;
        binFile >> arrDate;

        tNote = qUncompress(arrNote);
        tText = qUncompress(arrText);
        tDate = qUncompress(arrDate);

        listTasks.insert(tNote, QPair<QString, QDateTime>(tText, QDateTime().fromSecsSinceEpoch(tDate.toULongLong())));
    }

    fileNotes.close();
    return true;

}
