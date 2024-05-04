#ifndef CAVE_H
#define CAVE_H

#include "enemy.h"
#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

class Cave
{
    int _id;
    std::string _name;
    int _gold = 0;
    QSqlQuery query;
public:
    Cave(int caveID);
    std::vector<Enemy> makeCave();
    int getGold();
};

#endif // CAVE_H
