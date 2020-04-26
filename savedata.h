#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <QFile>
#include <QMap>
#include <QDataStream>
#include <QDir>
#include <QDebug>

class SaveData
{
    QString userName = "";
public:
    SaveData();

    void Save(QMap<QString, QPair<QString, QDateTime>> *listTasks);

};

#endif // SAVEDATA_H
