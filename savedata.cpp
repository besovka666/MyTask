#include "savedata.h"
#include <QDateTime>

#define FOLDER "/HoffmannTasks"
#define NOTES_FILE "/Tasks_data.svk"

SaveData::SaveData()
{
    userName = QDir::homePath();
    QDir().mkdir(userName + FOLDER);
}

void SaveData::Save(QMap<QString, QPair<QString, QDateTime>> *listTasks)
{
    //Переменная для хранения кол-ва элементов
    int count = listTasks->count();

    //Создаем файл
    QFile fileNotes(userName + FOLDER + NOTES_FILE);

    //Открываем файл
    if (fileNotes.open(QIODevice::WriteOnly)){

        //Открываем файл в бинарном режиме
        QDataStream binFile(&fileNotes);

        //Сначала записываем кол-во элементов
        binFile << count;

        QByteArray keyArr;
        QByteArray lavueArr;
        QByteArray dateArr;

        //Потом записываем все значения, заметок и текста, в бинарный файл
        for (int i = 0; i < count; i++){
            keyArr = listTasks->keys().at(i).toUtf8();
            lavueArr = listTasks->values().at(i).first.toUtf8();
            dateArr = QString::number(listTasks->values().at(i).second.toSecsSinceEpoch()).toUtf8();
            binFile << qCompress(keyArr) << qCompress(lavueArr) << qCompress(dateArr);
        }



    }

    fileNotes.close();

}
