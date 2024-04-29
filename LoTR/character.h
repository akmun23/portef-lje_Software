#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

class Character
{
private:
    std::string _name;
    int _hp;
    int _strength;

public:
    QSqlQuery query;

    Character(std::string name, int hp, int strength);
    void createHero();
    std::string getName();
    int getHp();
    int getStrength();
    void setName(std::string name);
    void setHp(int hp);
    void setStrength(int strength);
};


#endif // CHARACTER_H
