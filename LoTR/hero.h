#ifndef HERO_H
#define HERO_H

#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <string>

class Hero
{
private:
    std::string _name;
    int _xp = 0;
    int _hp = 10;
    int _level = 1;
    int _strength = 2;

public:
    QSqlQuery query;
    Hero(int heroID);
    Hero(std::string name);
    ~Hero();
    void levelUp();
    std::string getName();
    int getHp();
    int getStrength();
    void giveXp(int xp);
};

#endif // HERO_H
