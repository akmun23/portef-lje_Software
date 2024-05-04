#ifndef HERO_H
#define HERO_H

#include "character.h"
#include "enemy.h"
#include <string>
#include <iostream>

class Hero : public Character
{
private:
    int _xp = 0;
    int _level = 1;
    int _gold = 0;

public:
    Hero(int heroID, std::string name, int hp, int strength);
    Hero(std::string name);
    ~Hero();
    void fight(Enemy enemy);
    void getXp(int xp);
    void getGold(int gold);
    void levelUp();
};

#endif // HERO_H
