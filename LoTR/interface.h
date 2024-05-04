#ifndef INTERFACE_H
#define INTERFACE_H

#include "enemy.h"
#include "hero.h"
#include "cave.h"
#include<iostream>
#include<vector>
#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

class Interface
{
    QSqlQuery query;
    QSqlDatabase db;
    std::vector<Enemy> enemies;
    std::vector<Hero> heroes;
    int _currHero;
    int _currEnemy;
    bool _bossBattle = false;

public:
    Interface(QSqlDatabase &db);
    bool heroSelection();
    bool gameChoice();
    bool singleBattle();
    bool caveBattle();
    void gameLoop();
};

#endif // INTERFACE_H
