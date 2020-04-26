#ifndef LOADDATA_H
#define LOADDATA_H

#include <QFile>
#include <QMap>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include <QPair>

class LoadData
{
    QString userName;
    QMap<QString, QPair<QString, QDateTime>> listTasks;

public:
    LoadData();

    bool Load();
    QMap<QString, QPair<QString, QDateTime>> getlistTasks() const;
};

#endif // LOADDATA_H
