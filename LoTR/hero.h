#ifndef HERO_H
#define HERO_H

#include "character.h"
#include <string>

class Hero : public Character
{
private:
    int _xp = 0;
    int _level = 1;

public:
    Hero(int heroID, std::string name, int hp, int strength);
    Hero(std::string name);
    ~Hero();
    void getXp(int xp);
    void levelUp();
};

#endif // HERO_H
