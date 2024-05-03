#ifndef INTERFACE_H
#define INTERFACE_H

#include "enemy.h"
#include "hero.h"
#include<iostream>
#include<vector>
#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

class Interface
{
    QSqlQuery query;
    std::vector<Enemy> enemies;
    std::vector<Hero> heroes;
    int _currHero;
    int _currEnemy;
    QSqlDatabase db;
public:
    Interface(QSqlDatabase &db);
    void heroSelection();
    void singleBattle();
};

#endif // INTERFACE_H
