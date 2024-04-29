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
public:
    Interface();
    void heroSelection();
    void battle();
};

#endif // INTERFACE_H
